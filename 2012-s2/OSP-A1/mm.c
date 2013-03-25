/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"



/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
/* number of free lists */
#define FREE_LISTS 11

/*minimum size of a block */
#define MIN_SIZE 128
/*different block with different size */
#define CLASS0 256
#define CLASS1 521
#define CLASS2 1024
#define CLASS3 2048
#define CLASS4 4096
#define CLASS5 8192
#define CLASS6 16384
#define CLASS7 32768
#define CLASS8 65536
#define CLASS9 131072

/* set different block */
#define BLOCK1 1
#define BLOCK2 2
#define BLOCK3 3
#define BLOCK4 4
#define BLOCK5 5
#define BLOCK6 6
#define BLOCK7 7
#define BLOCK8 8
#define BLOCK9 9
#define BLOCK10 10

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

/* get the header's size */
#define HEADER_SIZE (sizeof(header))
/* get the footer's size */
#define FOOTER_SIZE (sizeof(footer))
/* get the get rounds up ALIGN's size */
#define SIZE_T_SIZE (ALIGN(HEADER_SIZE + FOOTER_SIZE))
/* get the minimum block's size */
#define MIN_BLOCK_SIZE (ALIGN(MIN_SIZE + SIZE_T_SIZE))
/* get the bp's size */
#define SIZE(bp) (bp->size & 0xfffffffe)
/* get the body's size */
#define BODY_SIZE(size) (size - SIZE_T_SIZE)

/* set it alloc */
#define MARK_ALLOC(bp) (bp->size = SIZE(bp))
/* set it free */
#define MARK_FREE(bp) (bp->size = bp->size | 1)

/* get the header */
#define HEADER(bp) ((header *)((char *)(bp) - HEADER_SIZE));
/* get the body */
#define BODY(bp) ((void *)((char *)(bp) + HEADER_SIZE))
/* get the footer */
#define FOOTER(bp) ((footer *)((char *)(bp) + (SIZE(bp) - FOOTER_SIZE)))
/* get the prev bp */
#define PREV(bp) (bp->prev)
/* get the next bp */
#define NEXT(bp) (FOOTER(bp)->next)

/* get the block */
#define BLOCK(bp,size) ((header *)((char *)bp + size))
/* get the prev block */
#define PREV_BLOCK(bp) ((header *)((char *)bp - ((footer *)((char *)bp - FOOTER_SIZE))->size))
/* get the next block */
#define NEXT_BLOCK(bp) ((header *)((char *)bp + SIZE(bp)))

/* check if the block is valid to use */
#define CHECK_BLOCK(bp) ((char *)bp >= (char *)mem_heap_lo() && (char *)bp <= (char *)mem_heap_hi())
/* check if bp is free now */
#define CHECK_FREE(bp) (bp->size & 1)

/* have not found valid block */
#define NOT_FOUND (void*)-1

/* block's header */
typedef struct header {
	size_t size;			
    /* pointer to prev free block */
	struct header *prev;	
} header;

/* block's footer */
typedef struct footer {
	size_t size;			
    /* pointer to next free block */
	struct header *next;	
} footer;
/* gloable segregated free lists  */
static header *segregate_list[FREE_LISTS];
static header *coalesced(header *bp);
static header *find_fit(size_t size);
static int chose_block(size_t size);
static void remove_from_segregate_list(header *bp);


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
	int i;
	for (i = 0; i < FREE_LISTS; i++)
		segregate_list[i] = 0;
	return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
    /* ignore spurious requests */
	if (size <= 0)
		return NULL;
    /* search the free list for a fit */
	header *bp = find_fit(size);
	if (bp == NOT_FOUND){
        /* get a new block */
		if (size < MIN_SIZE)
			size = MIN_SIZE;
		size = ALIGN(size + SIZE_T_SIZE);
		header *p = mem_sbrk(size);
		if (p == NOT_FOUND)
			return NULL;
		p->size = size;
		FOOTER(p)->size = size;
		bp = p;
	}
    /* set the block be alloced */
	MARK_ALLOC(bp);
	bp->prev = 0;
	FOOTER(bp)->next = 0;
	return BODY(bp);
}


/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {
    int i;
	header *bp = HEADER(ptr);
    /* set the block be free */
	i = chose_block(SIZE(bp));
	MARK_FREE(bp);
	bp->prev = 0;
    /* put it into the segregate list */
	FOOTER(bp)->next = segregate_list[i];
	if (segregate_list[i] != 0)
		segregate_list[i]->prev = bp;
	segregate_list[i] = bp;
	coalesced(bp);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
    int i;
    header *old_block = HEADER(ptr);
    size_t newsize = ALIGN(size + SIZE_T_SIZE);
    size_t oldsize = SIZE(old_block);
    /*if oldptr is NULL, then this is just malloc */
	if (ptr == NULL) 
		return mm_malloc(size);
    /*if size <= 0 then this is just free, return null */
	else if (size <= 0) {
		mm_free(ptr);
		return NULL;
	}
    /* realloc the block or return the a suitable new block */
	else {
		if (size < MIN_SIZE)
			size = MIN_SIZE;
        /* coalesce for a new block if old one not suitable */
		if (oldsize < newsize) {
            /* chack the prev and next bolck for suit one */
			header *prev = PREV_BLOCK(old_block);
			header *next = NEXT_BLOCK(old_block);
			if (!CHECK_BLOCK(next) || !CHECK_FREE(next) || next == old_block)
				next = 0;
			if (!CHECK_BLOCK(prev) || !CHECK_FREE(prev) || prev == old_block)
				prev = 0;
			if (prev != 0 && next != 0 && SIZE(prev) + SIZE(old_block) + SIZE(next) < newsize) {
				prev = 0;
				next = 0;
			} else if (prev != 0 && SIZE(prev) + SIZE(old_block) < newsize) {
				prev = 0;
			} else if (next != 0 && SIZE(old_block) + SIZE(next) < newsize) {
				next = 0;
			}
			header *new_block;
            /*can not found the block for coalesced */
			if (next == 0 && prev == 0) {
				new_block = HEADER(mm_malloc(newsize));
                /* copy the data to a new blcok*/
				memcpy(BODY(new_block),BODY(old_block),BODY_SIZE(oldsize));
                /* free the old block */
				mm_free(BODY(old_block));
                /* set the new block be alloced */
				MARK_ALLOC(new_block);
				new_block->prev = 0;
				FOOTER(new_block)->next = 0;
				return BODY(new_block);
			}
			else {
                /* set the olc block be free */
				i = chose_block(SIZE(old_block));
				MARK_FREE(old_block);
				old_block->prev = 0;
                /* put it into the segregate list */
				FOOTER(old_block)->next = segregate_list[i];
				if (segregate_list[i] != 0)
					segregate_list[i]->prev = old_block;
				segregate_list[i] = old_block;
                /* remove the new blcok form the segregate list */
				new_block = coalesced(old_block);
				remove_from_segregate_list(new_block);
                /* copy the data to a new blcok*/
				if (new_block != old_block)
					memcpy(BODY(new_block),BODY(old_block),BODY_SIZE(oldsize));
                /* set the new block be alloced */
				MARK_ALLOC(new_block);
				new_block->prev = 0;
				FOOTER(new_block)->next = 0;
				return BODY(new_block);
			}
		}
		else 
            /* the old block are big enough */
			return BODY(old_block);
	}
}

/*
 * chose_block - get the which part of the block need used in segregated free list
 */
static int chose_block(size_t size) {
	if (size < CLASS0)
		return 0;
	else if (size < CLASS1)
		return BLOCK1;
	else if (size < CLASS2)
		return BLOCK2;
	else if (size < CLASS3)
		return BLOCK3;
	else if (size < CLASS4)
		return BLOCK4;
	else if (size < CLASS5)
		return BLOCK5;
	else if (size < CLASS6)
		return BLOCK6;
	else if (size < CLASS7)
		return BLOCK7;
	else if (size < CLASS8)
		return BLOCK8;
	else if (size < CLASS9)
		return BLOCK9;
	else
		return BLOCK10;
}

/*
 * coaleseced - Boundary ta coalescing. retru ptr to coalesced block
 */
static header *coalesced(header *bp) {
    int i;
	size_t newsize;
	header *new_block;
	header *prev = PREV_BLOCK(bp);
	header *next = NEXT_BLOCK(bp);
    /* chack the prev and next bolck for suit one */
	if (!CHECK_BLOCK(prev) || !CHECK_FREE(prev) || prev == bp)
		prev = 0;
	if (!CHECK_BLOCK(next) || !CHECK_FREE(next) || next == bp)
		next = 0;
    /* case 1 - prev and next blocks are allocated */
	if (prev == 0 && next == 0)
		return bp;
	remove_from_segregate_list(bp);
    /* case 2 - prev block is free */
	if (prev && !next) {
		remove_from_segregate_list(prev);
		new_block = prev;
		newsize = SIZE(prev) + SIZE(bp);
	}
    /* case 3 - next block is free */
	else if (!prev && next) {
		remove_from_segregate_list(next);
		new_block = bp;
		newsize = SIZE(bp) + SIZE(next);
	}
    /* case 4 - prev and next are free */
	else {
		remove_from_segregate_list(prev);
		remove_from_segregate_list(next);
		new_block = prev;
		newsize = SIZE(prev) + SIZE(bp) + SIZE(next);
	}
	new_block->size = newsize;
	FOOTER(new_block)->size = newsize;
    /* set the olc block be free */
	i = chose_block(SIZE(new_block));
	MARK_FREE(new_block);
	new_block->prev = 0;
    /* put it into the segregate list */
	FOOTER(new_block)->next = segregate_list[i];
	if (segregate_list[i] != 0)
		segregate_list[i]->prev = new_block;
	segregate_list[i] = new_block;
	return new_block;
}

/*
 * find_fit - find the fit block for the new size
 * in the segregated free lists
 */
static header *find_fit(size_t size) {
	int i;
	header *bp;
	if (size <= 0)
		return NOT_FOUND;
	if (size < MIN_SIZE)
		size = MIN_SIZE;
	size = ALIGN(size + SIZE_T_SIZE);
    /* find the suitable size free block */
	for (i = chose_block(size); i < FREE_LISTS; i++) {
		bp = segregate_list[i];
		while (bp != 0 && SIZE(bp) < size)
			bp = NEXT(bp);
        /* remove the free block form the free list and spilt the block */
		if (bp != 0) {
			remove_from_segregate_list(bp);
            /* split the block */
			if (SIZE(bp) >= size && (SIZE(bp) - size) >= MIN_BLOCK_SIZE) {
			MARK_ALLOC(bp);
			header *new_block = BLOCK(bp,size);
			new_block->size = SIZE(bp) - size;
			FOOTER(new_block)->size = SIZE(bp) - size;
			bp->size = size;
			FOOTER(bp)->size = size;;
			i = chose_block(SIZE(new_block));
			MARK_FREE(new_block);
			new_block->prev = 0;
			FOOTER(new_block)->next = segregate_list[i];
			if (segregate_list[i] != 0)
				segregate_list[i]->prev = new_block;
			segregate_list[i] = new_block;
			coalesced(new_block);
			}
			return bp;
		}
	}
    /* not found any blcok */
	return NOT_FOUND;
}

/*
 * remove_from_segregate_list - remove the blloc form the 
 * segregate free list
 */
static void remove_from_segregate_list(header *bp) {
	int i;
    i = chose_block(SIZE(bp));
	if (PREV(bp) != 0)
		FOOTER(PREV(bp))->next = NEXT(bp);
	if (NEXT(bp) != 0)
		NEXT(bp)->prev = PREV(bp);
	if (segregate_list[i] == bp)
		segregate_list[i] = NEXT(bp);
}

