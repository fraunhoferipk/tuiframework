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


package app;
import view.*;
import configmodel.*;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.ContainerEvent;
import java.awt.event.ContainerListener;
import java.awt.event.InputMethodEvent;
import java.awt.event.InputMethodListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.Hashtable;
import java.util.Comparator;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JEditorPane;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;
import javax.swing.event.AncestorEvent;
import javax.swing.event.AncestorListener;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;

import com.l2fprod.common.propertysheet.PropertySheet;
import com.l2fprod.common.propertysheet.PropertySheetTable;
import com.l2fprod.common.propertysheet.PropertySheetTableModel;


/**
* This code was edited or generated using CloudGarden's Jigloo
* SWT/Swing GUI Builder, which is free for non-commercial
* use. If Jigloo is being used commercially (ie, by a corporation,
* company or business for any purpose whatever) then you
* should purchase a license for each developer using Jigloo.
* Please visit www.cloudgarden.com for details.
* Use of Jigloo implies acceptance of these licensing terms.
* A COMMERCIAL LICENSE HAS NOT BEEN PURCHASED FOR
* THIS MACHINE, SO JIGLOO OR THIS CODE CANNOT BE USED
* LEGALLY FOR ANY CORPORATE OR COMMERCIAL PURPOSE.
*/

/**
 * TestPropSheetDlg
 * 
 * @author Oliver Belaifa
 */
public class TestPropSheetDlg extends javax.swing.JDialog {
	private JEditorPane jEditorPane1;
	private JButton jButton1;
	private JPanel jPanel4;
	private JPanel jPanel3;
	private JPanel jPanel2;
	private JButton jButton2;
	private JPanel jPanel1;
	private ParameterGroupPanel parameterGroupPanel;
	private ParameterGroup pg;

	/**
	* Auto-generated main method to display this JDialog
	*/
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				JFrame frame = new JFrame();
				TestPropSheetDlg inst = new TestPropSheetDlg(frame);
				inst.setVisible(true);
			}
		});
	}
	
	public TestPropSheetDlg(JFrame frame) {
		super(frame);
		initGUI();
	}
	
	private void initGUI() {
		try {
			BoxLayout thisLayout = new BoxLayout(getContentPane(), javax.swing.BoxLayout.Y_AXIS);
			getContentPane().setLayout(thisLayout);
			this.setTitle("Property Editor");
			{
				Hashtable<String, String> parameterMap1 = new Hashtable<String, String>();
				parameterMap1.put("bla1", "34");
				parameterMap1.put("bla2", "Hallo");
				parameterMap1.put("bla3", "N1");
				parameterMap1.put("bla4", "4.112");
				
				Hashtable<String, String> parameterMap2 = new Hashtable<String, String>();
				parameterMap2.put("bla1", "34");
				parameterMap2.put("bla2", "Hallo");
				parameterMap2.put("bla3", "N1");
				parameterMap2.put("bla4", "4.112");
				
				
				Hashtable<String, String> parameterMap3 = new Hashtable<String, String>();
				parameterMap3.put("a3", "asdf");
				parameterMap3.put("c2", "bdf");
				parameterMap3.put("d5", "123");
				parameterMap3.put("e5", "12.112");
				
				Hashtable<String, String> parameterMap4 = new Hashtable<String, String>();
				parameterMap4.put("Test1", "10");
				parameterMap4.put("Scale", "1.5324");
				parameterMap4.put("Dst", "Native");

				
				ParameterGroup parameterGroup1 = new ParameterGroup("TestName");
				parameterGroup1.setParameterMap(parameterMap1);
				
				ParameterGroup parameterGroup2 = new ParameterGroup("Model");
				parameterGroup2.setParameterMap(parameterMap2);

				ParameterGroup parameterGroup3 = new ParameterGroup("Transf");
				parameterGroup3.setParameterMap(parameterMap3);
				
				ParameterGroup parameterGroup4 = new ParameterGroup("Main");
				parameterGroup4.setParameterMap(parameterMap4);
				
				Hashtable<String, ParameterGroup> parameterGroupMap1 = new Hashtable<String, ParameterGroup>();
				parameterGroupMap1.put(parameterGroup1.getName(), parameterGroup1);
				parameterGroupMap1.put(parameterGroup2.getName(), parameterGroup2);
				
				Hashtable<String, ParameterGroup> parameterGroupMap2 = new Hashtable<String, ParameterGroup>();
				parameterGroupMap2.put(parameterGroup3.getName(), parameterGroup3);
				
				Hashtable<String, ParameterGroup> parameterGroupMap3 = new Hashtable<String, ParameterGroup>();
				parameterGroupMap3.put(parameterGroup4.getName(), parameterGroup4);
				
				parameterGroup1.setParameterGroupMap(parameterGroupMap2);
				parameterGroup4.setParameterGroupMap(parameterGroupMap1);
				
				parameterGroupPanel = new ParameterGroupPanel();
				parameterGroupPanel.getTable().setFont(new Font("Arial", Font.PLAIN, 13));
				//parameterGroupPanel.setSortingProperties(true);
				
				parameterGroupPanel.setPreferredSize(new java.awt.Dimension(265, 236));
				getContentPane().add(parameterGroupPanel);
				//parameterGroupPanel.setPreferredSize(new java.awt.Dimension(390, 212));
				//parameterGroupPanel.setMode(PropertySheet.VIEW_AS_FLAT_LIST);
				//Comparator comp = new PropertySortComparator();
				//parameterGroupPanel.setSorting(true);
				//parameterGroupPanel.setPropertySortingComparator(comp);
				//parameterGroupPanel.setToolBarVisible(false);
				//parameterGroupPanel.setRestoreToggleStates(false);
				parameterGroupPanel.addParameterGroupMap(parameterGroupMap3);
				this.pg = parameterGroup3;
				
				
			}
			{
				jPanel4 = new JPanel();
				getContentPane().add(jPanel4);
				jPanel4.setMinimumSize(new java.awt.Dimension(6, 6));
				jPanel4.setPreferredSize(new java.awt.Dimension(6, 6));
			}
			{
				jPanel1 = new JPanel();
				BoxLayout jPanel1Layout = new BoxLayout(jPanel1, javax.swing.BoxLayout.X_AXIS);
				jPanel1.setLayout(jPanel1Layout);
				getContentPane().add(jPanel1);
				{
					jPanel2 = new JPanel();
					jPanel1.add(jPanel2);
					jPanel2.setPreferredSize(new java.awt.Dimension(13, 21));
				}
				{
					jButton1 = new JButton();
					jButton1.setLayout(null);
					jPanel1.add(jButton1);
					jButton1.setText("Apply");
					jButton1.setMinimumSize(new java.awt.Dimension(100, 27));
					jButton1.setPreferredSize(new java.awt.Dimension(100, 21));
					jButton1.setAlignmentX(0.5f);
					jButton1.addActionListener(new ActionListener() {
						public void actionPerformed(ActionEvent evt) {
							jButton1ActionPerformed(evt);
						}
					});
				}
				{
					jPanel3 = new JPanel();
					FlowLayout jPanel3Layout = new FlowLayout();
					jPanel3.setLayout(jPanel3Layout);
					jPanel1.add(jPanel3);
					jPanel3.setMaximumSize(new java.awt.Dimension(10, 10));
				}
				{
					jButton2 = new JButton();
					jPanel1.add(jButton2);
					jButton2.setText("Cancel");
					jButton2.setMinimumSize(new java.awt.Dimension(100, 27));
					jButton2.setPreferredSize(new java.awt.Dimension(100, 21));
					jButton2.setAlignmentX(0.5f);
				}
			}
			setSize(400, 300);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private void jButton1ActionPerformed(ActionEvent evt) {
		System.out.println("jButton1.actionPerformed, event="+evt);
		this.parameterGroupPanel.setRestoreToggleStates(true);
		PropertySheetTable table = this.parameterGroupPanel.getTable();
		PropertySheetTableModel model = (PropertySheetTableModel)table.getModel();
		Map map = parameterGroupPanel.getToggleStates();
		map.put("Main", Boolean.TRUE);
		map.put("Model", Boolean.TRUE);
		map.put("Transf", Boolean.TRUE);
		map.put("TestName", Boolean.TRUE);
		
		System.out.println("bla: " + table.getMinimumSize());
		
		parameterGroupPanel.setToggleStates(map);
		PropertySheetTableModel.Item item = model.getPropertySheetElement(0);
		if (item.hasToggle() && !item.isVisible()) {
			item.toggle();
		}
		
		System.out.println("bla: " + table.getMinimumSize());
		
		model.addTableModelListener(new TableModelListener() {

			@Override
			public void tableChanged(TableModelEvent arg0) {
				// TODO Auto-generated method stub
				tableChanged2(arg0);
			}
			
		});
		

		
		table.addInputMethodListener(new InputMethodListener() {
			 public void inputMethodTextChanged(InputMethodEvent event) {
				 inputMethodTextChanged(event);
			 }

			@Override
			public void caretPositionChanged(InputMethodEvent arg0) {
				// TODO Auto-generated method stub
				
			}
		});
		
		
		/*
		ParameterGroup parameterGroup2 = new ParameterGroup("Model");
		parameterGroup2.setParameterMap(parameterMap2);

		ParameterGroup parameterGroup3 = new ParameterGroup("Transf");
		parameterGroup3.setParameterMap(parameterMap3);
		
		ParameterGroup parameterGroup4 = new ParameterGroup("Main");
		*/
		/*
		Set<String> keys = map.keySet();
		Iterator<String> iter = keys.iterator();
		while (iter.hasNext()) {
			String key = iter.next();
			map.put(key, Boolean.TRUE);
		}
		*/
		

		//TODO add your code for jButton1.actionPerformed
	}

	
	
	public void inputMethodTextChanged(InputMethodEvent event) {
		System.out.println("jButton1.propertyChange, event="+event);
	}
	
	public void tableChanged2(TableModelEvent event) {
		System.out.println("TableModelEvent, event="+event);
		int height = (int)this.parameterGroupPanel.getTable().getMinimumSize().getHeight();
		parameterGroupPanel.setSize(new Dimension((int)this.parameterGroupPanel.getSize().getWidth(), height));
	}
	
	
}
