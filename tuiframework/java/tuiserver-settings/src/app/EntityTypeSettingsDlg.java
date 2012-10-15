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

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import javax.swing.BoxLayout;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.ListModel;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;
import view.ParameterGroupPanel;

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
 * EntityTypeSettingsDlg
 * 
 * @author Oliver Belaifa
 */
public class EntityTypeSettingsDlg extends javax.swing.JDialog {
	private JSplitPane jSplitPane1;
	private JPanel jPanel1;
	private JButton jButton1;
	private JButton jButton2;
	private JPanel jPanel7;
	private ParameterGroupPanel parameterGroupPanel;
	private JLabel jLabel4;
	private JSeparator jSeparator1;
	private JPanel jPanel6;
	private JPanel jPanel5;
	private JList jList1;
	private JPanel jPanel4;
	private JPanel jPanel3;
	private JPanel jPanel2;

	/**
	* Auto-generated main method to display this JDialog
	*/
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				JFrame frame = new JFrame();
				EntityTypeSettingsDlg inst = new EntityTypeSettingsDlg(frame);
				inst.setVisible(true);
			}
		});
	}
	
	public EntityTypeSettingsDlg(JFrame frame) {
		super(frame);
		initGUI();
	}
	
	private void initGUI() {
		try {
			BoxLayout thisLayout = new BoxLayout(getContentPane(), javax.swing.BoxLayout.Y_AXIS);
			getContentPane().setLayout(thisLayout);
			{
				jPanel3 = new JPanel();
				BoxLayout jPanel3Layout = new BoxLayout(jPanel3, javax.swing.BoxLayout.Y_AXIS);
				jPanel3.setLayout(jPanel3Layout);
				getContentPane().add(jPanel3);
				jPanel3.setPreferredSize(new java.awt.Dimension(609, 422));
				jPanel3.setAlignmentX(0.0f);
				jPanel3.setAlignmentY(0.0f);
				{
					jSplitPane1 = new JSplitPane();
					jPanel3.add(jSplitPane1);
					jSplitPane1.setDividerLocation(200);
					jSplitPane1.setPreferredSize(new java.awt.Dimension(580, 425));
					{
						ListModel jList1Model = 
							new DefaultComboBoxModel(
									new String[] { "Item One", "Item Two" });
						jList1 = new JList();
						jSplitPane1.add(jList1, JSplitPane.LEFT);
						jList1.setModel(jList1Model);
					}
					{
						jPanel5 = new JPanel();
						BoxLayout jPanel5Layout = new BoxLayout(jPanel5, javax.swing.BoxLayout.Y_AXIS);
						jPanel5.setLayout(jPanel5Layout);
						jSplitPane1.add(jPanel5, JSplitPane.RIGHT);
						{
							jPanel7 = new JPanel();
							jPanel5.add(jPanel7);
							{
								jButton2 = new JButton();
								jPanel7.add(jButton2);
								jButton2.setText("Change Image...");
							}
						}
						{
							jSeparator1 = new JSeparator();
							jPanel5.add(jSeparator1);
						}
						{
							jPanel6 = new JPanel();
							jPanel5.add(jPanel6, "left");
							BorderLayout jPanel6Layout = new BorderLayout();
							jPanel6.setLayout(jPanel6Layout);
							{
								jLabel4 = new JLabel();
								jPanel6.add(jLabel4, BorderLayout.NORTH);
								jLabel4.setText("Default Parameter");
								jLabel4.setHorizontalAlignment(SwingConstants.CENTER);
							}
							{
								parameterGroupPanel = new ParameterGroupPanel();
								jPanel6.add(parameterGroupPanel, BorderLayout.CENTER);
							}
						}
					}
				}
				{
					jPanel1 = new JPanel();
					BoxLayout jPanel1Layout = new BoxLayout(jPanel1, javax.swing.BoxLayout.X_AXIS);
					jPanel3.add(jPanel1);
					jPanel1.setLayout(jPanel1Layout);
					jPanel1.setAlignmentX(0.0f);
					{
						jPanel2 = new JPanel();
						jPanel1.add(jPanel2);
						jPanel2.setPreferredSize(new java.awt.Dimension(525, 20));
					}
					{
						jButton1 = new JButton();
						FlowLayout jButton1Layout = new FlowLayout();
						jButton1.setLayout(jButton1Layout);
						jPanel1.add(jButton1);
						jButton1.setText("OK");
						jButton1.setPreferredSize(new java.awt.Dimension(88, 20));
						jButton1.setAlignmentY(0.0f);
						jButton1.setVerifyInputWhenFocusTarget(false);
					}
					{
						jPanel4 = new JPanel();
						jPanel1.add(jPanel4);
						jPanel4.setAlignmentX(0.0f);
						jPanel4.setAlignmentY(0.0f);
						jPanel4.setPreferredSize(new java.awt.Dimension(2, 20));
					}
				}
			}
			this.setSize(520, 480);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
