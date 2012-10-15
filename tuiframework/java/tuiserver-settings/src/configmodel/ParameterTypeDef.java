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

import java.util.Hashtable;

/**
 * ParameterTypeDef
 * 
 * @author Oliver Belaifa
 */
public class ParameterTypeDef {
	protected String name;
	protected Hashtable<String, String> parameterTypeMap;
	protected String description;

	public ParameterTypeDef(String name, Hashtable<String, String> parameterTypeMap, String description) {
		this.name = name;
		this.parameterTypeMap = parameterTypeMap;
		this.description = description;
	}
	
	public String getName() {
		return this.name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public Hashtable<String, String> getParameterTypeMap() {
		return this.parameterTypeMap;
	}
	
	public void setParameterTypeMap(Hashtable<String, String> parameterTypeMap) {
		this.parameterTypeMap = parameterTypeMap;
	}
	
	public void setDescription(String description) {
		this.description = description;
	}
	
	public String getDescription() {
		return this.description;
	}
}
