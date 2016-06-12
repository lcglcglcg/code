/**
 * 
 */
package com.agrantsem.tracking.utility;

import java.util.HashMap;
import java.util.Map;

/**
 * @author wangzhuo
 *
 */
public class IpParser {
	
	 private static String[] tierOneDes = new String[] { "其他", "北京", "上海", "天津", "重庆", "河北", "山西", "辽宁", "吉林", "黑龙江"
         , "江苏", "浙江", "安徽", "福建", "江西", "山东", "河南", "湖北", "湖南", "广东", "海南", "四川", "贵州"
         , "云南", "陕西", "甘肃", "青海", "台湾", "内蒙古", "西藏", "广西", "宁夏", "新疆", "香港", "澳门"};
//	 private static int[] tierOneID = new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
//         , 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22
//         , 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34};
	 private static Map<String,Integer> regionMap=new HashMap<String,Integer>();
	 static{
		 for (int i = 0; i < tierOneDes.length; i++) {
			 regionMap.put(tierOneDes[i], i);
		 }
	 }
	 
	 //通过ip取到RegionId
     public static int GetRegionID (String country) {
    	 int regionId = 0;
         if(regionMap.containsKey(country)){
        	 regionId=regionMap.get(country);
         }
         return regionId;
     }
     
     public static int getIpCount(){
    	 return 35;
     }
}
