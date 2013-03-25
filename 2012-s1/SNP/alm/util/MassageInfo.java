package alm.util;
import java.util.*;
import java.io.*;




public class MassageInfo implements Serializable  {
		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;
		public String timeInfo;
		public ArrayList<ClientDeail> clientList;
		public int pointer;
		public MassageInfo(int pointer,String timeInfo, ArrayList<ClientDeail> clientList) {
			this.clientList = clientList;
			this.timeInfo = timeInfo;
			this.pointer=pointer;
		}
	}
	