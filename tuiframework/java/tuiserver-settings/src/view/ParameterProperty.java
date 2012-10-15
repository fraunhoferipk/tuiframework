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


package view;

import java.util.Hashtable;

import com.l2fprod.common.propertysheet.DefaultProperty;

/**
 * ParameterProperty
 * 
 * @author Oliver Belaifa
 */
public class ParameterProperty extends DefaultProperty implements Comparable<ParameterProperty> {

	Hashtable<String, String> parameterMap;
	protected String key;
	
	public void set(Hashtable<String, String> parameterMap, String key) {
		this.parameterMap = parameterMap;
		this.key = key;
	}
	
	
	public void update(String value) {
		this.parameterMap.put(this.key, value);
	}
	
	// Implementation of Comparable interface
	public int compareTo(ParameterProperty p){
		return (this.getDisplayName()).compareToIgnoreCase(p.getDisplayName());
	}

	public int compare(ParameterProperty a, ParameterProperty b	){
		return a.compareTo(b);
	}

	   
}
