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

import configmodel.*;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Map;
import java.util.Set;

import javax.swing.table.TableRowSorter;

import configmodel.*;

import com.l2fprod.common.propertysheet.*;


/**
 * ParameterGroupPanel
 * 
 * @author Oliver Belaifa
 */
public class ParameterGroupPanel  extends PropertySheetPanel {
	
	public ParameterGroupPanel() {
		PropertySheetTable table = this.getTable();
		PropertySheetTableModel model = (PropertySheetTableModel)table.getSheetModel();
		TableRowSorter sorter = new TableRowSorter(model);
		table.setRowSorter(sorter);
		this.setToolBarVisible(false);
		this.setSorting(true);
	}
	
	
	private void addParameterGroup(ParameterProperty parent, ParameterGroup pg) {
		Hashtable<String, ParameterGroup> parameterGroupMap = pg.getParameterGroupMap();
		
		Enumeration<ParameterGroup> e = parameterGroupMap.elements();
		while (e.hasMoreElements()) {
			ParameterGroup pg2 = e.nextElement();
			ParameterProperty pp = this.addProperties(pg2);
			pp.setDisplayName(pg2.getName());
			pp.setShortDescription(pg2.getName() + "shortDescription");
			pp.setName(pg.getName());
			parent.addSubProperty(pp);
		}
	}
	
	
	private void addParameter(ParameterProperty parent, ParameterGroup pg) {
		Hashtable<String, String> parameterMap = pg.getParameterMap();
		
		Enumeration<String> e = parameterMap.keys();
		while (e.hasMoreElements()) {
			String key = e.nextElement();
			String value = parameterMap.get(key);
			
			ParameterProperty pp = new ParameterProperty();
			pp.setDisplayName(key);
			pp.setShortDescription(key + "shortDescription");
			pp.setEditable(true);
			pp.setName(key);
			pp.setType(String.class);
			pp.setValue(value);
			pp.set(parameterMap, key);
			parent.addSubProperty(pp);
			pp.addPropertyChangeListener(new PropertyChangeListener() {
				public void propertyChange(PropertyChangeEvent evt) {
					propertyChanged(evt);
				}
			});
		}
	}
	
	private ParameterProperty addProperties(ParameterGroup pg) {
		
		ParameterProperty pp = new ParameterProperty();
		pp.setDisplayName(pg.getName());
		pp.setShortDescription(pg.getName() + "shortDescription");
		this.addParameterGroup(pp, pg);
		this.addParameter(pp, pg);
		
		//Here we sort the subProperties of this ParameterProperty
		Property[] subs = pp.getSubProperties();
		if (subs.length > 0) {
			pp.clearSubProperties();
			Arrays.sort(subs);
			pp.addSubProperties(subs);
		}
		
		//Hashtable<String, ParameterGroupMap> pm = pg.getParameterMap();
		return pp;
	}
	
	
	public void addParameterGroupMap(Hashtable<String, ParameterGroup> parameterGroupMap) {
		Enumeration<ParameterGroup> e = parameterGroupMap.elements();
		while (e.hasMoreElements()) {
			ParameterGroup pg = e.nextElement();	
			ParameterProperty pp = this.addProperties(pg);
			this.addProperty(pp);
		}
		this.expand();
	}
	
	public void expand() {
		PropertySheetTable table = this.getTable();
		PropertySheetTableModel model = (PropertySheetTableModel)table.getModel();
		model.getPropertyCount();
		int i = 0;
		
		while (i < model.getRowCount()) {
			PropertySheetTableModel.Item item = model.getPropertySheetElement(i);
			if (item.hasToggle() && !item.isVisible()) {
				item.toggle();
			}
			++i;
		}
	   //sorter.sort();
	   

	}
	
	
	
	private void propertyChanged(PropertyChangeEvent evt) {
		System.out.println("jButton1.propertyChange, event="+evt);
		ParameterProperty pp = (ParameterProperty)evt.getSource();
		pp.update((String)pp.getValue());
		System.out.println("" + pp.getDisplayName() + ": " + pp.getValue());
		
	}

}
