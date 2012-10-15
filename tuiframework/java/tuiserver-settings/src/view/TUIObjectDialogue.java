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

import java.util.*;
import java.util.List;
import java.lang.*;
import javax.swing.*;
import javax.swing.GroupLayout.*;
import javax.swing.event.TableModelListener;
import javax.swing.table.*;
import java.awt.*;
import java.awt.event.*;

import configmodel.EntityType;
import configmodel.EntityID;
import configmodel.Port;
import configmodel.Port.DataFlowDirection;
import view.Playground;


public class TUIObjectDialogue extends JDialog implements ActionListener {
	private TUIObjectTableModel tableModel;
	private JTable portTable;
	private JTextField nameField;
	private JTextField descField;
	private Playground owner;
	
	public TUIObjectDialogue(Playground parent) {
		super();
		owner = parent;
		initGUI();
	}
	
	public void initGUI() {
		this.setTitle("Create New TUIObjectType");
		this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		this.setSize(400,400);

		GroupLayout layout = new GroupLayout(this.getContentPane());
		this.getContentPane().setLayout(layout);
		layout.setAutoCreateGaps(true);
		layout.setAutoCreateContainerGaps(true);
		
		// create name label
		JLabel nameLabel = new JLabel();
		nameLabel.setAlignmentX(LEFT_ALIGNMENT);
		nameLabel.setText("TUIObjectType Name: ");
		
		// create name text field
		nameField = new JTextField(20);
		nameField.setAlignmentX(RIGHT_ALIGNMENT);
		
		// create description label
		JLabel descLabel = new JLabel();
		descLabel.setAlignmentX(LEFT_ALIGNMENT);
		descLabel.setText("TUIObjectType Description: ");
		
		// create description text field
		descField = new JTextField(20);
		descField.setAlignmentX(RIGHT_ALIGNMENT);
		
		// create port config label
		JLabel tableLabel = new JLabel();
		tableLabel.setAlignmentX(LEFT_ALIGNMENT);
		tableLabel.setText("Port Configuration: ");
		
		//Create Port Table
		tableModel = new TUIObjectTableModel();
		portTable = new JTable(tableModel);
		JScrollPane scrollPane = new JScrollPane(portTable);
		scrollPane.setPreferredSize(new Dimension(700, 300));
		scrollPane.setAlignmentX(LEFT_ALIGNMENT);
		
		// Add buttons
		final JButton addRowButton = new JButton("Add Port");
		addRowButton.setActionCommand("Add Port");
		addRowButton.addActionListener(this);
		addRowButton.setAlignmentX(LEFT_ALIGNMENT);
		final JButton removeRowButton = new JButton("Remove Selection");
		removeRowButton.setActionCommand("Remove Selection");
		removeRowButton.addActionListener(this);
		removeRowButton.setAlignmentX(LEFT_ALIGNMENT);
        final JButton cancelButton = new JButton("Cancel");
        cancelButton.addActionListener(this);
        final JButton setButton = new JButton("Save");
        setButton.setActionCommand("Save");
        setButton.addActionListener(this);
        
		JPanel buttonPane = new JPanel();
		buttonPane.setLayout(new BoxLayout(buttonPane, BoxLayout.LINE_AXIS));
		buttonPane.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));
		buttonPane.add(addRowButton);
		buttonPane.add(removeRowButton);
		buttonPane.add(Box.createHorizontalGlue());
		buttonPane.add(cancelButton);
		buttonPane.add(Box.createRigidArea(new Dimension(10, 0)));
		buttonPane.add(setButton);
		
		
		// create layout
		ParallelGroup hGroup = layout.createParallelGroup();
		hGroup.addGroup(layout.createSequentialGroup().addComponent(nameLabel).addComponent(nameField));
		hGroup.addGroup(layout.createSequentialGroup().addComponent(descLabel).addComponent(descField));
		hGroup.addComponent(tableLabel);
		hGroup.addComponent(scrollPane);
		hGroup.addComponent(buttonPane);
		
		SequentialGroup vGroup = layout.createSequentialGroup();
		vGroup.addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE).addComponent(nameLabel).addComponent(nameField));
		vGroup.addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE).addComponent(descLabel).addComponent(descField));
		vGroup.addComponent(tableLabel);
		vGroup.addComponent(scrollPane);
		vGroup.addComponent(buttonPane);
				
		layout.setHorizontalGroup(hGroup);
		layout.setVerticalGroup(vGroup);
		
		this.pack();
		this.setLocationRelativeTo(null);
		this.setVisible(true);
	}
	
	public boolean validateConfiguration() {
		
		// test if the new type has a name
		if (nameField.getText().trim().equals("")) {
			JOptionPane.showMessageDialog((Component)this.getContentPane(), "The TUIObjectType must have a name.", "Cofiguration Error", JOptionPane.WARNING_MESSAGE);
			return false;
		}
		
		// test if there is at least one port
		if (tableModel.data.size() == 0) {
			JOptionPane.showMessageDialog((Component)this.getContentPane(), "The TUIObjectType must have at least one port.", "Cofiguration Error", JOptionPane.WARNING_MESSAGE);
			return false;
		}
		
		
		ArrayList<String> portNames = new ArrayList<String>();
		for (int i = 0; i < tableModel.data.size(); i++) {
			
			// test if all ports have a name
			String portName = (String)tableModel.data.get(i)[0];
			if (portName.trim().equals("")) {
				JOptionPane.showMessageDialog((Component)this.getContentPane(), "All ports must have a name.", "Cofiguration Error", JOptionPane.WARNING_MESSAGE);
				return false;
			}
			
			// test if all ports have a port type
			String portType = (String)tableModel.data.get(i)[1];
			if (portType.trim().equals("")) {
				JOptionPane.showMessageDialog((Component)this.getContentPane(), "All ports must have a Port Type.", "Cofiguration Error", JOptionPane.WARNING_MESSAGE);
				return false;
			}
			
			portNames.add(portName);
		}
		
		// test if all port names are unique
		Set<String> portNameSet = new HashSet<String>(portNames);
		if (portNameSet.size() != portNames.size()) {
			JOptionPane.showMessageDialog((Component)this.getContentPane(), "All port names must be unique.", "Cofiguration Error", JOptionPane.WARNING_MESSAGE);
			return false;
		}
		
		return true;
		
	}
	
	//Handle clicks on buttons
    public void actionPerformed(ActionEvent e) {
        if ("Save".equals(e.getActionCommand())) {
        	// First we validate the configuration
        	if (!validateConfiguration()) {
        		return;
        	}
        	
        	// Next we create a new EntityType for this TUIObjectType
        	String name = nameField.getText();
        	String desc = descField.getText();
        	
        	Hashtable<String, Port> portMap = new Hashtable<String, Port>();
        	
        	Iterator<Object[]> it = tableModel.data.iterator();
        	while (it.hasNext()){
        		Object[] row = it.next();
        		
        		Port.DataFlowDirection dfd;
        		if (((Boolean)row[3]).booleanValue()) {
        			dfd = Port.DataFlowDirection.Source;
        		} else {
        			dfd = Port.DataFlowDirection.Sink;
        		}
        		
        		Port newPort = new Port((String)row[0], (String)row[1], dfd, (String)row[2]);
        		
        		portMap.put((String)row[0], newPort);
        	}
        	
        	// Last we add this new TUIObjectType to Playground data model
        	EntityType newTUIObjectType = new EntityType(new EntityID(name, EntityID.Type.TUI), portMap, desc);
        	this.owner.addTUIObjectTypeToModel(newTUIObjectType);
        	
        	this.dispose();
        	
        } else if ("Cancel".equals(e.getActionCommand())) {
        	this.dispose();
        } else if ("Add Port".equals(e.getActionCommand())) {
        	this.tableModel.addNewRow();
        } else if ("Remove Selection".equals(e.getActionCommand())) {
        	int[] selectedRows = portTable.getSelectedRows();
        	this.tableModel.removeRows(selectedRows);
        }
    }
	
	class TUIObjectTableModel extends AbstractTableModel {
		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;
		private String[] colNames = {"Port Name","Port Type","Port Description","Source","Sink"};
		public ArrayList<Object[]> data;
		//private Object[][] data = {{"port name","port type","port description",new Boolean(false), new Boolean(false)}};
		
		public TUIObjectTableModel() {
			//super();
			Object[] new_row = {"port name","port type","port description",new Boolean(true), new Boolean(false)};
			this.data = new ArrayList<Object[]>();
			this.data.add(new_row);
		}
		
		public int getColumnCount() {
			return colNames.length;
		}
		
		public int getRowCount() {
			return data.size();
		}
		
		public String getColumnName(int col) {
			return colNames[col];
		}
		
		public Object getValueAt(int row, int col) {
			return data.get(row)[col];
		}
		
		public Class getColumnClass(int col) {
			return getValueAt(0, col).getClass();
		}
		
		public boolean isCellEditable(int row, int col) {
			return true;
		}
		
		public void setValueAt(Object value, int row, int col) {
			if (col == 3) {		//i.e. one of the Source or Sink columns was edited
				data.get(row)[3] = value;
				data.get(row)[4] = new Boolean(!((Boolean)value).booleanValue());
			} else if (col == 4) {
				data.get(row)[4] = value;
				data.get(row)[3] = new Boolean(!((Boolean)value).booleanValue());
			} else {
				data.get(row)[col] = value;
			}
			fireTableDataChanged();
		}
		
		public void addNewRow() {
			Object[] new_row = {"port name","port type","port description",new Boolean(true), new Boolean(false)};
			data.add(new_row);
			fireTableDataChanged();
		}
		
		public void removeRows(int[] rows) {
			ArrayList<Object[]> list = new ArrayList<Object[]>();
			for (int i = 0; i < rows.length; i++) {
				list.add(data.get(rows[i]));
			}
			data.removeAll(list);
			fireTableDataChanged();
		}
	}
}

