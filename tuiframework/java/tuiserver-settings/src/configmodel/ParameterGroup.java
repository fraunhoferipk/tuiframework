/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    
    The TUIFramework library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    The TUIFramework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with the TUIFramework.  If not, see <http://www.gnu.org/licenses/>.
*/


package configmodel;

import java.util.Enumeration;
import java.util.Hashtable;

/**
 * ParameterGroup
 * 
 * @author Oliver Belaifa
 */
public class ParameterGroup {
	protected String name;
	protected Hashtable<String, String> parameterMap = new Hashtable<String, String>();
	protected Hashtable<String, ParameterGroup> parameterGroupMap = new Hashtable<String, ParameterGroup>();
	
	public ParameterGroup(String name) {
		this.name = name;
	}

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}
	
	public Hashtable<String, String> getParameterMap() {
		return this.parameterMap;
	}
	
	public void setParameterMap(Hashtable<String, String> parameterMap) {
		this.parameterMap = parameterMap;
	}
		
	public Hashtable<String, ParameterGroup> getParameterGroupMap() {
		return this.parameterGroupMap;
	}
	
	public void setParameterGroupMap(Hashtable<String, ParameterGroup> parameterGroupMap) {
		this.parameterGroupMap = parameterGroupMap;
	}
	
	public Object clone() {
		Hashtable<String, String> clonedParameterMap = cloneParameterMap(this.parameterMap);
		Hashtable<String, ParameterGroup> clonedParameterGroupMap = cloneParameterGroupMap(this.parameterGroupMap);
		
		ParameterGroup parameterGroup = new ParameterGroup(this.name);
		parameterGroup.setParameterMap(clonedParameterMap);
		parameterGroup.setParameterGroupMap(clonedParameterGroupMap);
		return parameterGroup;
	}
	
	
	public static Hashtable<String, String> cloneParameterMap(Hashtable<String, String> original) {
		 Hashtable<String, String> copy = new  Hashtable<String, String>();
		 
		 Enumeration<String> en = original.keys();
		 while (en.hasMoreElements()) {
			 String key = en.nextElement();
			 String value = original.get(key);
			 copy.put(key, value);
		 }
		 
		 return copy;
	}
	
	
	public static Hashtable<String, ParameterGroup> cloneParameterGroupMap(Hashtable<String, ParameterGroup> original) {
		 Hashtable<String, ParameterGroup> copy = new  Hashtable<String, ParameterGroup>();
		 Enumeration<String> en = original.keys();
		 while (en.hasMoreElements()) {
			 String key = en.nextElement();
			 ParameterGroup value = (ParameterGroup)original.get(key).clone();
			 copy.put(key, value);
		 }
		 
		 return copy;
	}
}

