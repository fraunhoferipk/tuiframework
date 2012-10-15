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

/**
 * EntityID
 * 
 * @author Oliver Belaifa
 */
public class EntityID {
	
	public enum Type {
		DEV,
		MSP,
		TUI 
	}

	private String name;
	private Type type;

	
	public EntityID(String name, Type type) {
		this.name = name;
		this.type = type;
	}

	public String getName() {
		return this.name;
	}

	public Type getType() {
		return this.type;
	}
	
	public String getTypeString() {
		switch (this.type) {
		case DEV: return "device";
		case MSP: return "MSP";
		case TUI: return "TUIObject";
		}
		return "";
	}

	@Override
	public boolean equals(Object obj) {
		return type.equals(((EntityID)obj).getType()) && name.equals(((EntityID)obj).getName());
	}

	@Override
	public int hashCode() {
		return this.type.hashCode()^this.name.hashCode();
	}
}
