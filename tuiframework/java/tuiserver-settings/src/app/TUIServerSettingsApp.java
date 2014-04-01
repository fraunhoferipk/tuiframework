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
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.io.File;
import java.util.Hashtable;

import javax.swing.filechooser.FileFilter;

import javax.swing.*;
import javax.swing.border.BevelBorder;
import javax.swing.border.LineBorder;
import javax.swing.border.SoftBevelBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.MenuKeyEvent;
import javax.swing.event.MenuKeyListener;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.table.TableRowSorter;

import com.l2fprod.common.propertysheet.PropertySheetTable;
import com.l2fprod.common.propertysheet.PropertySheetTableModel;

import project.Project;

import configmodel.EntityID;
import configmodel.EntityType;
import configmodel.InternalException;
import configmodel.ParameterGroup;
import configmodel.ServerConfig;
import test.TestServerConfig;
import view.ParameterGroupPanel;
import view.Playground;
import view.ServerConfigView;
import xml.Exporter;
import xml.Importer;

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
 * TUIServerSettingsApp
 * 
 * @author Oliver Belaifa
 */
public class TUIServerSettingsApp extends javax.swing.JFrame {

	private JMenuItem helpMenuItem;
	private JMenu jMenu5;
	private JList tuiTypeList;
	private JLabel jLabel1;
	private JPanel jPanel3;
	private JList mspTypeList;
	private JList deviceTypeList;
	private JPanel jPanel1;
	private JSplitPane jSplitPane1;
	private JMenuItem deleteSelectionMenuItem;
	private JSeparator jSeparator6;
	private JMenuItem jMenuItem8;
	private JMenuItem jMenuItem7;
	private JPopupMenu jEntityTypePopupMenu1;
	private JSeparator jSeparator4;
	private JMenuItem jMenuItem4;
	private JSeparator jSeparator3;
	private JMenuItem jMenuItem3;
	private JMenuItem jMenuItem2;
	private JMenuItem jMenuItem1;
	private JLabel jLabel4;
	private JPanel jPanel2;
	private ParameterGroupPanel parameterGroupPanel;
	private JSplitPane jSplitPane2;
	private JMenuItem changeNameMenuItem;
	private JMenuItem newTUIObjectMenuItem;
	private JSeparator jSeparator1;
	private JMenu jMenu4;
	private JMenuItem exitMenuItem;
	private JSeparator jSeparator2;
	private JMenuItem importConfigMenuItem;
	private JMenuItem exportConfigMenuItem;
	private Playground playground1;
	private JLabel jLabel3;
	private JPanel jPanel5;
	private JLabel jLabel2;
	private JPanel jPanel4;
	private JMenu jMenu3;
	private JMenuBar jMenuBar1;
	
	private JScrollPane scrollPane1;
	private JScrollPane scrollPane2;
	private JScrollPane scrollPane3;
	private JScrollPane scrollPane4;
	
	public Project project = new Project();
	private Exporter exporter = new Exporter();
	private Importer importer = new Importer();
	private EntityType contextMenuSelectedEntityType = null;
	private JList contextMenuSelectedList = null;
	private JSeparator jSeparator5;
	private JMenuItem jMenuItem5;
	private JMenuItem jMenuItem11;
	private int contextMenuSelectedIndex = -1;
	private EntityID contextMenuEntityTypeID = null;
	private JMenuItem jMenuItem10;
	private JPopupMenu jEntityTypeTitlePopupMenu1;
	private JMenuItem jMenuItem9;
	

	/**
	* Auto-generated main method to display this JFrame
	*/
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				TUIServerSettingsApp inst = new TUIServerSettingsApp();
				inst.setLocationRelativeTo(null);
				inst.setVisible(true);
			}
		});
	}
	
	public TUIServerSettingsApp() {
		super();
		initGUI();
		this.initApp();
	}
	
	
	private void initApp() {
		this.project = new Project();
		this.project.setDefaultImages();
		//this.serverConfig = TestServerConfig.createServerConfig();
		//this.serverConfigView = TestServerConfig.createServerConfigView();
		//this.workSpace.getServerConfigView();
		this.playground1.setProject(this.project, this.deviceTypeList, this.mspTypeList, this.tuiTypeList);
	}
	
	private void initGUI() {
		try {
			{
			}
			BoxLayout thisLayout = new BoxLayout(getContentPane(), javax.swing.BoxLayout.Y_AXIS);
			getContentPane().setLayout(thisLayout);
			this.setTitle("TUI Server Settings");
			{
				jSplitPane1 = new JSplitPane();
				getContentPane().add(jSplitPane1);
				jSplitPane1.setOrientation(JSplitPane.VERTICAL_SPLIT);
				jSplitPane1.setPreferredSize(new java.awt.Dimension(178, 23));
				jSplitPane1.setInheritsPopupMenu(true);
				jSplitPane1.setDividerLocation(100);
				{
					jPanel1 = new JPanel();
					jSplitPane1.add(jPanel1, JSplitPane.LEFT);
					GridLayout jPanel1Layout = new GridLayout(1, 1);
					jPanel1Layout.setHgap(5);
					jPanel1Layout.setVgap(5);
					jPanel1Layout.setColumns(1);
					jPanel1.setLayout(jPanel1Layout);
					{
						jPanel3 = new JPanel();
						BorderLayout jPanel3Layout = new BorderLayout();
						jPanel3.setLayout(jPanel3Layout);
						jPanel1.add(jPanel3);
						jPanel3.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
						{
							jLabel1 = new JLabel();
							jPanel3.add(jLabel1, BorderLayout.NORTH);
							BorderLayout jLabel1Layout = new BorderLayout();
							jLabel1.setLayout(jLabel1Layout);
							jLabel1.setText("Device types");
							jLabel1.setPreferredSize(new java.awt.Dimension(77, 14));
							jLabel1.setHorizontalAlignment(SwingConstants.CENTER);
							jLabel1.addMouseListener(new MouseAdapter() {
								public void mousePressed(MouseEvent evt) {
									System.out.println("deviceTypeListLabel.mousePressed, event="+evt);
									if (evt.getButton() == MouseEvent.BUTTON3) {
										contextMenuEntityTypeID = new EntityID("", EntityID.Type.DEV);
										contextMenuSelectedList = deviceTypeList;
										getJEntityTypeTitlePopupMenu1().show(jLabel1, evt.getX(), evt.getY());
									}
								}
							});
						}
						{
							ListModel jList2Model = 
								new DefaultComboBoxModel(
										new String[] { "Item One", "Item Two" });
							deviceTypeList = new JList();
							scrollPane3 = new JScrollPane(deviceTypeList);
							scrollPane3.setPreferredSize(new java.awt.Dimension(100, 120));
							jPanel3.add(scrollPane3, BorderLayout.CENTER);
							BorderLayout jList2Layout = new BorderLayout();
							deviceTypeList.setLayout(jList2Layout);
							deviceTypeList.setModel(jList2Model);
							deviceTypeList.setBorder(BorderFactory.createTitledBorder(""));
							deviceTypeList.addMouseListener(new MouseAdapter() {
								public void mouseReleased(MouseEvent evt) {
									deviceTypeListMouseReleased(evt);
								}
								public void mousePressed(MouseEvent evt) {
									System.out.println("deviceTypeList.mousePressed, event="+evt);
									if (evt.getButton() == MouseEvent.BUTTON3) {
										int index = deviceTypeList.locationToIndex(evt.getPoint());
										if (index >= 0) {
											contextMenuSelectedList = deviceTypeList;
											contextMenuSelectedIndex = index;
											contextMenuSelectedEntityType = (EntityType)deviceTypeList.getModel().getElementAt(index);
											System.out.println(contextMenuSelectedEntityType.getEntityID().getName());
											getJMenuItem8().setEnabled( ! project.getServerConfig().hasInstancedEntityType(contextMenuSelectedEntityType.getEntityID()));
											getJEntityTypePopupMenu1().show(deviceTypeList, evt.getX(), evt.getY());
										} else {
											contextMenuSelectedEntityType = null;
										}
									}
								}
							});
							deviceTypeList.addListSelectionListener(new ListSelectionListener() {
								public void valueChanged(ListSelectionEvent evt) {
									deviceTypeListValueChanged(evt);
								}
							});
						}
					}
					{
						jPanel4 = new JPanel();
						BorderLayout jPanel4Layout = new BorderLayout();
						jPanel4.setLayout(jPanel4Layout);
						jPanel1.add(jPanel4);
						jPanel4.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
						{
							jLabel2 = new JLabel();
							BorderLayout jLabel2Layout = new BorderLayout();
							jLabel2.setLayout(jLabel2Layout);
							jPanel4.add(jLabel2, BorderLayout.NORTH);
							jLabel2.setText("MSP types");
							jLabel2.setHorizontalAlignment(SwingConstants.CENTER);
							jLabel2.addMouseListener(new MouseAdapter() {
								public void mousePressed(MouseEvent evt) {
									System.out.println("mspTypeListLabel.mousePressed, event="+evt);
									if (evt.getButton() == MouseEvent.BUTTON3) {
										contextMenuEntityTypeID = new EntityID("", EntityID.Type.MSP);
										contextMenuSelectedList = mspTypeList;
										getJEntityTypeTitlePopupMenu1().show(jLabel2, evt.getX(), evt.getY());
									}
								}
							});
						}
						{
							ListModel jList3Model = 
								new DefaultComboBoxModel(
										new String[] { "Item One", "Item Two" });
							mspTypeList = new JList();
							scrollPane4 = new JScrollPane(mspTypeList);
							scrollPane4.setPreferredSize(new java.awt.Dimension(100, 120));
							jPanel4.add(scrollPane4, BorderLayout.CENTER);
							BorderLayout jList3Layout = new BorderLayout();
							mspTypeList.setLayout(jList3Layout);
							mspTypeList.setModel(jList3Model);
							mspTypeList.setMaximumSize(new java.awt.Dimension(0, 0));
							mspTypeList.setBorder(BorderFactory.createTitledBorder(""));
							mspTypeList.addMouseListener(new MouseAdapter() {
								public void mouseReleased(MouseEvent evt) {
									mspTypeListMouseReleased(evt);
								}
								public void mousePressed(MouseEvent evt) {
									System.out.println("mspTypeList.mousePressed, event="+evt);
									if (evt.getButton() == MouseEvent.BUTTON3) {
										int index = mspTypeList.locationToIndex(evt.getPoint());
										if (index >= 0) {
											contextMenuSelectedList = mspTypeList;
											contextMenuSelectedIndex = index;
											contextMenuSelectedEntityType = (EntityType)mspTypeList.getModel().getElementAt(index);
											System.out.println(contextMenuSelectedEntityType.getEntityID().getName());
											getJMenuItem8().setEnabled( ! project.getServerConfig().hasInstancedEntityType(contextMenuSelectedEntityType.getEntityID()));
											getJEntityTypePopupMenu1().show(mspTypeList, evt.getX(), evt.getY());
										} else {
											contextMenuSelectedEntityType = null;
										}
									}
								}
							});
							mspTypeList.addListSelectionListener(new ListSelectionListener() {
								public void valueChanged(ListSelectionEvent evt) {
									mspTypeListValueChanged(evt);
								}
							});
						}
					}
					{
						jPanel5 = new JPanel();
						BorderLayout jPanel5Layout = new BorderLayout();
						jPanel5.setLayout(jPanel5Layout);
						jPanel1.add(jPanel5);
						jPanel5.setBorder(BorderFactory.createBevelBorder(BevelBorder.LOWERED));
						{
							jLabel3 = new JLabel();
							BorderLayout jLabel3Layout = new BorderLayout();
							jLabel3.setLayout(jLabel3Layout);
							jPanel5.add(jLabel3, BorderLayout.NORTH);
							jLabel3.setText("TUI object types");
							jLabel3.setHorizontalAlignment(SwingConstants.CENTER);
							jLabel3.addMouseListener(new MouseAdapter() {
								public void mousePressed(MouseEvent evt) {
									System.out.println("tuiTypeListLabel.mousePressed, event="+evt);
									if (evt.getButton() == MouseEvent.BUTTON3) {
										contextMenuEntityTypeID = new EntityID("", EntityID.Type.TUI);
										contextMenuSelectedList = tuiTypeList;
										getJEntityTypeTitlePopupMenu1().show(jLabel3, evt.getX(), evt.getY());
									}
								}
							});
						}
						{
							tuiTypeList = new JList();
							scrollPane2 = new JScrollPane(tuiTypeList);
							scrollPane2.setPreferredSize(new java.awt.Dimension(100, 120));
							jPanel5.add(scrollPane2, BorderLayout.CENTER);
							BorderLayout jList1Layout = new BorderLayout();
							tuiTypeList.setLayout(jList1Layout);
							tuiTypeList.setBorder(BorderFactory.createTitledBorder(""));
							tuiTypeList.addMouseListener(new MouseAdapter() {
								public void mouseReleased(MouseEvent evt) {
									tuiTypeListMouseReleased(evt);
								}
								public void mousePressed(MouseEvent evt) {
									System.out.println("tuiTypeList.mousePressed, event="+evt);
									if (evt.getButton() == MouseEvent.BUTTON3) {
										int index = tuiTypeList.locationToIndex(evt.getPoint());
										if (index >= 0) {
											contextMenuSelectedList = tuiTypeList;
											contextMenuSelectedIndex = index;
											contextMenuSelectedEntityType = (EntityType)tuiTypeList.getModel().getElementAt(index);
											System.out.println(contextMenuSelectedEntityType.getEntityID().getName());
											getJMenuItem8().setEnabled( ! project.getServerConfig().hasInstancedEntityType(contextMenuSelectedEntityType.getEntityID()));
											getJEntityTypePopupMenu1().show(tuiTypeList, evt.getX(), evt.getY());
										} else {
											contextMenuSelectedEntityType = null;
										}
									}
								}
							});
							tuiTypeList.addListSelectionListener(new ListSelectionListener() {
								public void valueChanged(ListSelectionEvent evt) {
									tuiTypeListValueChanged(evt);
								}
							});
						}
					}
				}
				jSplitPane1.add(getJSplitPane2(), JSplitPane.RIGHT);
			}
			this.setSize(964, 611);
			{
				jMenuBar1 = new JMenuBar();
				setJMenuBar(jMenuBar1);
				{
					jMenu3 = new JMenu();
					jMenuBar1.add(jMenu3);
					jMenu3.setText("File");
					{
						importConfigMenuItem = new JMenuItem();
						jMenu3.add(getJMenuItem3());
						jMenu3.add(getJMenuItem1());
						jMenu3.add(getJSeparator4());
						jMenu3.add(getJMenuItem2());
						jMenu3.add(getJMenuItem4());
						jMenu3.add(getJSeparator3());
						jMenu3.add(getJMenuItem11());
						jMenu3.add(importConfigMenuItem);
						importConfigMenuItem.setText("Import Configuration...");
						importConfigMenuItem.addActionListener(new ActionListener() {
							public void actionPerformed(ActionEvent evt) {
								importConfigMenuItemActionPerformed(evt);
							}
						});
					}
					{
						exportConfigMenuItem = new JMenuItem();
						jMenu3.add(exportConfigMenuItem);
						exportConfigMenuItem.setText("Export Configuration...");
						exportConfigMenuItem.addActionListener(new ActionListener() {
							public void actionPerformed(ActionEvent evt) {
								exportConfigMenuItemActionPerformed(evt);
							}
						});
					}
					{
						jSeparator2 = new JSeparator();
						jMenu3.add(jSeparator2);
					}
					{
						exitMenuItem = new JMenuItem();
						jMenu3.add(exitMenuItem);
						exitMenuItem.setText("Exit");
						exitMenuItem.addActionListener(new ActionListener() {
							public void actionPerformed(ActionEvent evt) {
								exitMenuItemActionPerformed(evt);
							}
						});
					}
				}
				{
					jMenu4 = new JMenu();
					jMenuBar1.add(jMenu4);
					jMenu4.setText("Edit");
					{
						changeNameMenuItem = new JMenuItem();
						jMenu4.add(changeNameMenuItem);
						jMenu4.add(getJSeparator5());
						jMenu4.add(getJMenuItem5());
						changeNameMenuItem.setText("Change EntityInstance Name...");
						changeNameMenuItem.addActionListener(new ActionListener() {
							public void actionPerformed(ActionEvent evt) {
								changeNameMenuItemActionPerformed(evt);
							}
						});
					}
					{
						jSeparator1 = new JSeparator();
						jMenu4.add(jSeparator1);
					}
					{
						newTUIObjectMenuItem = new JMenuItem();
						jMenu4.add(newTUIObjectMenuItem);
						newTUIObjectMenuItem.setText("Create new TUIObject Type...");
						newTUIObjectMenuItem.addActionListener(new ActionListener() {
							public void actionPerformed(ActionEvent evt) {
								createNewTUIObjectTypeActionPerformed(evt);
							}
						});
					}
					{
						jSeparator1 = new JSeparator();
						jMenu4.add(jSeparator1);
					}
					{
						deleteSelectionMenuItem = new JMenuItem();
						jMenu4.add(deleteSelectionMenuItem);
						deleteSelectionMenuItem.setText("Delete Selection");
						deleteSelectionMenuItem.addActionListener(new ActionListener() {
							public void actionPerformed(ActionEvent evt) {
								deleteSelectionMenuItemActionPerformed(evt);
							}
						});
						deleteSelectionMenuItem.addMenuKeyListener(new MenuKeyListener() {
							public void menuKeyTyped(MenuKeyEvent evt) {
								System.out.println("deleteSelectionMenuItem.menuKeyTyped, event="+evt);
								//TODO add your code for deleteSelectionMenuItem.menuKeyTyped
							}
							public void menuKeyReleased(MenuKeyEvent evt) {
								System.out.println("deleteSelectionMenuItem.menuKeyReleased, event="+evt);
								//TODO add your code for deleteSelectionMenuItem.menuKeyReleased
							}
							public void menuKeyPressed(MenuKeyEvent evt) {
								deleteSelectionMenuItemMenuKeyPressed(evt);
							}
						});
						deleteSelectionMenuItem.addMouseListener(new MouseAdapter() {
							public void mouseClicked(MouseEvent evt) {
								deleteSelectionMenuItemMouseClicked(evt);
							}
						});
					}
				}
				{
					jMenu5 = new JMenu();
					jMenuBar1.add(jMenu5);
					jMenu5.setText("Help");
					{
						helpMenuItem = new JMenuItem();
						jMenu5.add(helpMenuItem);
						helpMenuItem.setText("About...");
						helpMenuItem.addActionListener(new ActionListener() {
							public void actionPerformed(ActionEvent evt) {
								helpMenuItemActionPerformed(evt);
							}
						});
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private void playground1MouseMoved(MouseEvent evt) {
		this.playground1.mouseMoved(evt.getPoint());
		//this.playground1.repaint();
		//System.out.println("playground1.mouseMoved, event="+evt);
		//TODO add your code for playground1.mouseMoved
	}
	
	private void playground1MousePressed(MouseEvent evt) {
		System.out.println("playground1.mousePressed, event="+evt);
		this.playground1.mousePressed(evt.getPoint(), evt.getButton());
		//TODO add your code for playground1.mousePressed
	}
	
	private void playground1MouseReleased(MouseEvent evt) {
		System.out.println("playground1.mouseReleased, event="+evt);
		this.playground1.mouseReleased(evt.getPoint());
		//TODO add your code for playground1.mouseReleased
	}
	
	private void playground1MouseDragged(MouseEvent evt) {
//		System.out.println("playground1.mouseDragged, event="+evt);
		this.playground1.mouseDragged(evt.getPoint());
		//TODO add your code for playground1.mouseDragged
	}
	
	private void deleteSelectionMenuItemMouseClicked(MouseEvent evt) {
		System.out.println("deleteSelectionMenuItem.mouseClicked, event="+evt);
		//TODO add your code for deleteSelectionMenuItem.mouseClicked
	}
	
	private void deleteSelectionMenuItemMenuKeyPressed(MenuKeyEvent evt) {
		System.out.println("deleteSelectionMenuItem.menuKeyPressed, event="+evt);
		//TODO add your code for deleteSelectionMenuItem.menuKeyPressed
	}
	
	private void deleteSelectionMenuItemActionPerformed(ActionEvent evt) {
		System.out.println("deleteSelectionMenuItem.actionPerformed, event="+evt);
		//TODO add your code for deleteSelectionMenuItem.actionPerformed
		this.playground1.deleteSelection();
	}
	
	private void mspTypeListValueChanged(ListSelectionEvent evt) {
		System.out.println("mspTypeList.valueChanged, event="+evt);
		/*
		if ( ! this.deviceTypeList.isSelectionEmpty()) {
			this.mspTypeList.clearSelection();
			this.tuiTypeList.clearSelection();
		}
		*/
	}
	
	private void deviceTypeListValueChanged(ListSelectionEvent evt) {
	
		System.out.println("deviceTypeList.valueChanged, event="+evt);
		/*
		if ( ! this.mspTypeList.isSelectionEmpty()) {
			this.deviceTypeList.clearSelection();
			this.tuiTypeList.clearSelection();
		}
		*/
	}
	
	private void tuiTypeListValueChanged(ListSelectionEvent evt) {
		System.out.println("tuiTypeList.valueChanged, event="+evt);
		/*
		if ( ! this.tuiTypeList.isSelectionEmpty()) {
			this.mspTypeList.clearSelection();
			this.deviceTypeList.clearSelection();
		}
		*/
	}
	
	private void deviceTypeListMouseReleased(MouseEvent evt) {
		System.out.println("deviceTypeList.mouseReleased, event="+evt);
		this.deviceTypeList.clearSelection();
	}
	
	private void mspTypeListMouseReleased(MouseEvent evt) {
		System.out.println("mspTypeList.mouseReleased, event="+evt);
		this.mspTypeList.clearSelection();
		//TODO add your code for mspTypeList.mouseReleased
	}
	
	private void tuiTypeListMouseReleased(MouseEvent evt) {
		System.out.println("tuiTypeList.mouseReleased, event="+evt);
		this.tuiTypeList.clearSelection();
		//TODO add your code for tuiTypeList.mouseReleased
	}
	
	public static String getExtension(File f) {
        String ext = "";
        String s = f.getName();
        int i = s.lastIndexOf('.');

        if (i > 0 &&  i < s.length() - 1) {
            ext = s.substring(i+1).toLowerCase();
        }
        return ext;
    }
	
	private File currentExportDirectory = null;
	
	private void exportConfigMenuItemActionPerformed(ActionEvent evt) {
		System.out.println("exportConfigMenuItem.actionPerformed, event="+evt);
		
		JFileChooser fileChooser = new JFileChooser();
		if (this.currentExportDirectory != null) {
			fileChooser.setCurrentDirectory(this.currentExportDirectory);
		}
		FileFilter filter = new FileNameExtensionFilter("XML file", "xml", "XML");
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		fileChooser.removeChoosableFileFilter(fileChooser.getAcceptAllFileFilter());
		fileChooser.addChoosableFileFilter(filter);
		int result = fileChooser.showSaveDialog(this);
	    if(result == JFileChooser.CANCEL_OPTION) return;
	    this.currentExportDirectory = fileChooser.getCurrentDirectory();
	    File file = fileChooser.getSelectedFile();
	    this.exporter.exportServerConfig(file, this.project);
	    
	    String extension = getExtension(file);
	    if ( ! extension.toLowerCase().equals("xml")) {
	    	file = new File(file.getPath() + ".xml");
	    }
	}
	
	private File currentImportDirectory = null;
	
	private void importConfigMenuItemActionPerformed(ActionEvent evt) {
		System.out.println("importConfigMenuItem.actionPerformed, event="+evt);
		
		JFileChooser fileChooser = new JFileChooser();
		if (this.currentImportDirectory != null) {
			fileChooser.setCurrentDirectory(this.currentImportDirectory);
		}
		FileFilter filter = new FileNameExtensionFilter("XML file", "xml", "XML");
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		fileChooser.removeChoosableFileFilter(fileChooser.getAcceptAllFileFilter());
		fileChooser.addChoosableFileFilter(filter);
		int result = fileChooser.showOpenDialog(this);
	    if(result == JFileChooser.CANCEL_OPTION) return;
	    this.currentImportDirectory = fileChooser.getCurrentDirectory();
	    this.importer.importServerConfig(fileChooser.getSelectedFile(), this.project);
	    this.playground1.setProject(this.project, this.deviceTypeList, this.mspTypeList, this.tuiTypeList);
	    this.playground1.setReArrange(true);
	    this.project.getServerConfigView().getEntityInstancePositions().clear();
		this.playground1.repaint();
		//TODO add your code for importConfigMenuItem.actionPerformed
	}
	
	private void changeNameMenuItemActionPerformed(ActionEvent evt) {
		System.out.println("changeNameMenuItem.actionPerformed, event="+evt);
		this.playground1.changeEntityName();
		
		//TODO add your code for changeNameMenuItem.actionPerformed
	}
	
	private void createNewTUIObjectTypeActionPerformed(ActionEvent evt) {
		System.out.println("newTUIObjectMenuItem.actionPerformed, event="+evt);
		this.playground1.createNewTUIObjectType();
	}
	
	
	private void helpMenuItemActionPerformed(ActionEvent evt) {
		System.out.println("helpMenuItem.actionPerformed, event="+evt);
		JOptionPane.showMessageDialog(this.playground1, "TUI Server Settings - developed by Fraunhofer IPK", "About",  JOptionPane.INFORMATION_MESSAGE);

		//TODO add your code for helpMenuItem.actionPerformed
	}
	
	private void exitMenuItemActionPerformed(ActionEvent evt) {
		System.out.println("exitMenuItem.actionPerformed, event="+evt);
		dispose();
		System.exit(0);
	}
	
	private JSplitPane getJSplitPane2() {
		if(jSplitPane2 == null) {
			jSplitPane2 = new JSplitPane();
			jSplitPane2.setDividerLocation(200);
			{
				playground1 = new Playground(getParameterGroupPanel());
				playground1.setPreferredSize(new java.awt.Dimension(704, 393));
				playground1.setSize(709, 450);
				playground1.addMouseListener(new MouseAdapter() {
					public void mouseReleased(MouseEvent evt) {
						playground1MouseReleased(evt);
					}
					public void mousePressed(MouseEvent evt) {
						playground1MousePressed(evt);
					}
				});
				scrollPane1 = new JScrollPane(playground1);
				scrollPane1.getVerticalScrollBar().setUnitIncrement(32);
				jSplitPane2.add(scrollPane1, JSplitPane.RIGHT);
				jSplitPane2.add(getJPanel2(), JSplitPane.LEFT);
				playground1.addMouseMotionListener(new MouseMotionAdapter() {
					public void mouseDragged(MouseEvent evt) {
						playground1MouseDragged(evt);
					}
					public void mouseMoved(MouseEvent evt) {
						playground1MouseMoved(evt);
					}
				});
			}
		}
		return jSplitPane2;
	}

	private ParameterGroupPanel getParameterGroupPanel() {
		if(this.parameterGroupPanel == null) {
			parameterGroupPanel = new ParameterGroupPanel();

//			Hashtable<String, String> parameterMap1 = new Hashtable<String, String>();
//			parameterMap1.put("bla1", "34");
//			parameterMap1.put("bla2", "Hallo");
//			parameterMap1.put("bla3", "N1");
//			parameterMap1.put("bla4", "4.112");
//			
//			Hashtable<String, String> parameterMap2 = new Hashtable<String, String>();
//			parameterMap2.put("bla1", "34");
//			parameterMap2.put("bla2", "Hallo");
//			parameterMap2.put("bla3", "N1");
//			parameterMap2.put("bla4", "4.112");
//			
//			
//			Hashtable<String, String> parameterMap3 = new Hashtable<String, String>();
//			parameterMap3.put("a3", "asdf");
//			parameterMap3.put("c2", "bdf");
//			parameterMap3.put("d5", "123");
//			parameterMap3.put("e5", "12.112");
//			
//			Hashtable<String, String> parameterMap4 = new Hashtable<String, String>();
//			parameterMap4.put("Test1", "10");
//			parameterMap4.put("Scale", "1.5324");
//			parameterMap4.put("Dst", "Native");
//
//			
//			ParameterGroup parameterGroup1 = new ParameterGroup("TestName");
//			parameterGroup1.setParameterMap(parameterMap1);
//			
//			ParameterGroup parameterGroup2 = new ParameterGroup("Model");
//			parameterGroup2.setParameterMap(parameterMap2);
//
//			ParameterGroup parameterGroup3 = new ParameterGroup("Transf");
//			parameterGroup3.setParameterMap(parameterMap3);
//			
//			ParameterGroup parameterGroup4 = new ParameterGroup("Main");
//			parameterGroup4.setParameterMap(parameterMap4);
//			
//			Hashtable<String, ParameterGroup> parameterGroupMap1 = new Hashtable<String, ParameterGroup>();
//			parameterGroupMap1.put(parameterGroup1.getName(), parameterGroup1);
//			parameterGroupMap1.put(parameterGroup2.getName(), parameterGroup2);
//			
//			Hashtable<String, ParameterGroup> parameterGroupMap2 = new Hashtable<String, ParameterGroup>();
//			parameterGroupMap2.put(parameterGroup3.getName(), parameterGroup3);
//	
//			Hashtable<String, ParameterGroup> parameterGroupMap3 = new Hashtable<String, ParameterGroup>();
//			
//			parameterGroupMap3.put(parameterGroup3.getName(), parameterGroup3);
//			parameterGroupMap3.put(parameterGroup4.getName(), parameterGroup4);
//			parameterGroupMap3.put(parameterGroup1.getName(), parameterGroup1);
//			parameterGroupMap3.put(parameterGroup2.getName(), parameterGroup2);
//			
//			parameterGroup1.setParameterGroupMap(parameterGroupMap2);
//			parameterGroup4.setParameterGroupMap(parameterGroupMap1);
//			
//			parameterGroupPanel = new ParameterGroupPanel();
//			parameterGroupPanel.getTable().setFont(new Font("Arial", Font.PLAIN, 13));

			
			//this.add(parameterGroupPanel);
			
			//parameterGroupPanel.setSortingProperties(true);
			
			//parameterGroupPanel.setPreferredSize(new java.awt.Dimension(265, 236));
			//getContentPane().add(parameterGroupPanel);
			
			//parameterGroupPanel.setPreferredSize(new java.awt.Dimension(390, 212));
			//parameterGroupPanel.setMode(PropertySheet.VIEW_AS_FLAT_LIST);
			//Comparator comp = new PropertySortComparator();
			//parameterGroupPanel.setSorting(true);
			//parameterGroupPanel.setPropertySortingComparator(comp);
			//parameterGroupPanel.setToolBarVisible(false);
			//parameterGroupPanel.setRestoreToggleStates(false);
			
			//parameterGroupPanel.addParameterGroupMap(parameterGroupMap3);
			//parameterGroupPanel.addParameterGroupMap(parameterMap3);

		}
		return this.parameterGroupPanel;
	}
	
	private JPanel getJPanel2() {
		if(jPanel2 == null) {
			jPanel2 = new JPanel();
			BorderLayout jPanel2Layout = new BorderLayout();
			jPanel2.setLayout(jPanel2Layout);
			jPanel2.add(getJLabel4(), BorderLayout.NORTH);
			jPanel2.add(getParameterGroupPanel(), BorderLayout.CENTER);
		}
		return jPanel2;
	}
	
	private JLabel getJLabel4() {
		if(jLabel4 == null) {
			jLabel4 = new JLabel();
			jLabel4.setText("Parameter");
			jLabel4.setHorizontalAlignment(SwingConstants.CENTER);
		}
		return jLabel4;
	}
	
	private JMenuItem getJMenuItem1() {
		if(jMenuItem1 == null) {
			jMenuItem1 = new JMenuItem();
			jMenuItem1.setText("Open Project...");
			jMenuItem1.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jMenuItem1ActionPerformed(evt);
				}
			});
		}
		return jMenuItem1;
	}
	
	private JMenuItem getJMenuItem2() {
		if(jMenuItem2 == null) {
			jMenuItem2 = new JMenuItem();
			jMenuItem2.setText("Save Project");
			jMenuItem2.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jMenuItem2ActionPerformed(evt);
				}
			});
		}
		return jMenuItem2;
	}

	private JMenuItem getJMenuItem3() {
		if(jMenuItem3 == null) {
			jMenuItem3 = new JMenuItem();
			jMenuItem3.setText("New Project");
			jMenuItem3.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jMenuItem3ActionPerformed(evt);
				}
			});
		}
		return jMenuItem3;
	}
	
	private JSeparator getJSeparator3() {
		if(jSeparator3 == null) {
			jSeparator3 = new JSeparator();
		}
		return jSeparator3;
	}
	
	private JMenuItem getJMenuItem4() {
		if(jMenuItem4 == null) {
			jMenuItem4 = new JMenuItem();
			jMenuItem4.setText("Save Project as...");
			jMenuItem4.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jMenuItem4ActionPerformed(evt);
				}
			});
		}
		return jMenuItem4;
	}
	
	private JSeparator getJSeparator4() {
		if(jSeparator4 == null) {
			jSeparator4 = new JSeparator();
		}
		return jSeparator4;
	}

	private void jMenuItem6ActionPerformed(ActionEvent evt) {
		EntityTypeSettingsDlg dlg = new EntityTypeSettingsDlg(this);
		dlg.setModal(true);
		dlg.setVisible(true);
		System.out.println("jMenuItem6.actionPerformed, event="+evt);

		//TODO add your code for jMenuItem6.actionPerformed
	}
	
	private JPopupMenu getJEntityTypePopupMenu1() {
		if(jEntityTypePopupMenu1 == null) {
			jEntityTypePopupMenu1 = new JPopupMenu();
			jEntityTypePopupMenu1.add(getJMenuItem7());
			jEntityTypePopupMenu1.add(getJMenuItem9());
			jEntityTypePopupMenu1.add(getJSeparator6());
			jEntityTypePopupMenu1.add(getJMenuItem8());
		}
		return jEntityTypePopupMenu1;
	}
	
	/**
	* Auto-generated method for setting the popup menu for a component
	*/
	private void setComponentPopupMenu(final java.awt.Component parent, final javax.swing.JPopupMenu menu) {
		parent.addMouseListener(new java.awt.event.MouseAdapter() {
			public void mousePressed(java.awt.event.MouseEvent e) {
				if(e.isPopupTrigger())
					menu.show(parent, e.getX(), e.getY());
			}
			public void mouseReleased(java.awt.event.MouseEvent e) {
				if(e.isPopupTrigger())
					menu.show(parent, e.getX(), e.getY());
			}
		});
	}
	
	private JMenuItem getJMenuItem7() {
		if(jMenuItem7 == null) {
			jMenuItem7 = new JMenuItem();
			jMenuItem7.setText("Change Entity Type Image...");
			jMenuItem7.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jMenuItem7ActionPerformed(evt);
				}
			});
		}
		return jMenuItem7;
	}
	

	
	private JMenuItem getJMenuItem8() {
		if(jMenuItem8 == null) {
			jMenuItem8 = new JMenuItem();
			jMenuItem8.setText("Delete Entity Type");
			jMenuItem8.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jMenuItem8ActionPerformed(evt);
				}
			});
		}
		return jMenuItem8;
	}
	
	private JSeparator getJSeparator6() {
		if(jSeparator6 == null) {
			jSeparator6 = new JSeparator();
		}
		return jSeparator6;
	}
	
	private void jMenuItem7ActionPerformed(ActionEvent evt) {
		System.out.println("jMenuItem7.actionPerformed, event="+evt);
		//TODO add your code for jMenuItem7.actionPerformed
		JFileChooser fileChooser = new JFileChooser();
		FileFilter filter = new FileNameExtensionFilter("PNG file", "png", "PNG");
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		fileChooser.removeChoosableFileFilter(fileChooser.getAcceptAllFileFilter());
		fileChooser.addChoosableFileFilter(filter);
		int result = fileChooser.showOpenDialog(this);
	    if(result == JFileChooser.CANCEL_OPTION) return;
	    File file = fileChooser.getSelectedFile();
	    try {
	    	this.project.getServerConfigView().getImageDB().loadImage(file.getPath());
	    } catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	    try {
	    	this.project.getServerConfigView().getImageDB().setEntityTypeImage(this.contextMenuSelectedEntityType.getEntityID(), file.getPath());
	    }  catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
	    }

	    this.contextMenuSelectedList.repaint();
	    this.playground1.repaint();
	}
	
	private void jMenuItem8ActionPerformed(ActionEvent evt) {
		System.out.println("jMenuItem8.actionPerformed, event="+evt);
		if (contextMenuSelectedEntityType != null) {
			this.project.getEntityTypeDB().getEntityTypeMap().remove(contextMenuSelectedEntityType.getEntityID());
			
			DefaultListModel model = (DefaultListModel)this.contextMenuSelectedList.getModel();
			model.remove(contextMenuSelectedIndex);
			
			//this.project.getServerConfigView().getImageDB().getDefaultTypeImageMap().put(key, value);
			this.contextMenuSelectedList.validate();
		}
	}
	
	private JMenuItem getJMenuItem9() {
		if(jMenuItem9 == null) {
			jMenuItem9 = new JMenuItem();
			jMenuItem9.setText("Use Default Entity Type Image");
			jMenuItem9.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jMenuItem9ActionPerformed(evt);
				}
			});
		}
		return jMenuItem9;
	}
	
	private void jMenuItem9ActionPerformed(ActionEvent evt) {
		System.out.println("jMenuItem9.actionPerformed, event="+evt);
		this.project.getServerConfigView().getImageDB().removeEntityTypeImage(this.contextMenuSelectedEntityType.getEntityID());
	    this.contextMenuSelectedList.repaint();
	    this.playground1.repaint();
		//TODO add your code for jMenuItem9.actionPerformed
	}
	
	private JPopupMenu getJEntityTypeTitlePopupMenu1() {
		if(jEntityTypeTitlePopupMenu1 == null) {
			jEntityTypeTitlePopupMenu1 = new JPopupMenu();
			jEntityTypeTitlePopupMenu1.add(getJMenuItem10());
		}
		return jEntityTypeTitlePopupMenu1;
	}
	
	private JMenuItem getJMenuItem10() {
		if(jMenuItem10 == null) {
			jMenuItem10 = new JMenuItem();
			jMenuItem10.setText("Change Default Entity Type Image...");
			jMenuItem10.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jMenuItem10ActionPerformed(evt);
				}
			});
		}
		return jMenuItem10;
	}
	
	private void jMenuItem10ActionPerformed(ActionEvent evt) {
		System.out.println("jMenuItem10.actionPerformed, event="+evt);
		System.out.println("jMenuItem7.actionPerformed, event="+evt);
		//TODO add your code for jMenuItem7.actionPerformed
		JFileChooser fileChooser = new JFileChooser();
		FileFilter filter = new FileNameExtensionFilter("PNG file", "png", "PNG");
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		fileChooser.removeChoosableFileFilter(fileChooser.getAcceptAllFileFilter());
		fileChooser.addChoosableFileFilter(filter);
		int result = fileChooser.showOpenDialog(this);
	    if(result == JFileChooser.CANCEL_OPTION) return;
	    File file = fileChooser.getSelectedFile();
	    try {
	    	this.project.getServerConfigView().getImageDB().loadImage(file.getPath());
	    } catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	    try {
	    	this.project.getServerConfigView().getImageDB().setDefaultTypeImage(this.contextMenuEntityTypeID.getType(), file.getPath());
	    }  catch (InternalException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
	    }

	    this.contextMenuSelectedList.repaint();
	    this.playground1.repaint();
	}
	
	private JMenuItem getJMenuItem11() {
		if(jMenuItem11 == null) {
			jMenuItem11 = new JMenuItem();
			jMenuItem11.setText("New Configuration");
			jMenuItem11.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jMenuItem11ActionPerformed(evt);
				}
			});
		}
		return jMenuItem11;
	}
	
	private void jMenuItem3ActionPerformed(ActionEvent evt) {
		System.out.println("jMenuItem3.actionPerformed, event="+evt);
		//TODO add your code for jMenuItem3.actionPerformed
		// new project
		this.project.getServerConfig().clear();
		this.project.getEntityTypeDB().clear();
		this.playground1.setProject(this.project, this.deviceTypeList, this.mspTypeList, this.tuiTypeList);
		this.playground1.setReArrange(true);
		this.project.getServerConfigView().getEntityInstancePositions().clear();
		this.playground1.repaint();
	}
	
	private File currentReadProjectDirectory = null;
	
	private void jMenuItem1ActionPerformed(ActionEvent evt) {
		System.out.println("jMenuItem1.actionPerformed, event="+evt);
		//TODO add your code for jMenuItem1.actionPerformed
		// open project
		JFileChooser fileChooser = new JFileChooser();
		if (this.currentReadProjectDirectory != null) {
			fileChooser.setCurrentDirectory(this.currentReadProjectDirectory);
		}
		FileFilter filter = new FileNameExtensionFilter("TUIProj file", "tuiproj", "TUIProj");
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		fileChooser.removeChoosableFileFilter(fileChooser.getAcceptAllFileFilter());
		fileChooser.addChoosableFileFilter(filter);
		int result = fileChooser.showOpenDialog(this);
	    if(result == JFileChooser.CANCEL_OPTION) return;
	    this.currentReadProjectDirectory = fileChooser.getCurrentDirectory();
	    this.importer.readProject(fileChooser.getSelectedFile(), this.project);
	    this.playground1.setProject(this.project, this.deviceTypeList, this.mspTypeList, this.tuiTypeList);
	    this.playground1.setReArrange(true);
	    this.project.getServerConfigView().getEntityInstancePositions().clear();
		this.playground1.repaint();
	}
	
	private void jMenuItem2ActionPerformed(ActionEvent evt) {
		System.out.println("jMenuItem2.actionPerformed, event="+evt);
		//TODO add your code for jMenuItem2.actionPerformed
		// save project
	}
	

	private File currentWriteProjectDirectory = null;
	
	private void jMenuItem4ActionPerformed(ActionEvent evt) {
		System.out.println("jMenuItem4.actionPerformed, event="+evt);
		//TODO add your code for jMenuItem4.actionPerformed
		// save project as
		JFileChooser fileChooser = new JFileChooser();
		if (this.currentWriteProjectDirectory != null) {
			fileChooser.setCurrentDirectory(this.currentWriteProjectDirectory);
		}
		FileFilter filter = new FileNameExtensionFilter("TUIProj file", "tuiproj", "TUIProj");
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		fileChooser.removeChoosableFileFilter(fileChooser.getAcceptAllFileFilter());
		fileChooser.addChoosableFileFilter(filter);
		int result = fileChooser.showSaveDialog(this);
	    if(result == JFileChooser.CANCEL_OPTION) return;
	    this.currentWriteProjectDirectory = fileChooser.getCurrentDirectory();
	    File file = fileChooser.getSelectedFile();
	    
	    String extension = getExtension(file);
	    if ( ! extension.toLowerCase().equals("tuiproj")) {
	    	file = new File(file.getPath() + ".tuiproj");
	    }
	    
	    this.exporter.saveProject(file, this.project);
	}
	
	private void jMenuItem11ActionPerformed(ActionEvent evt) {
		System.out.println("jMenuItem11.actionPerformed, event="+evt);
		//TODO add your code for jMenuItem11.actionPerformed
		// new configuration
		this.project.getServerConfig().clear();
		this.playground1.setProject(this.project, this.deviceTypeList, this.mspTypeList, this.tuiTypeList);
		this.playground1.setReArrange(true);
		this.project.getServerConfigView().getEntityInstancePositions().clear();
		this.playground1.repaint();
	}
	
	private JMenuItem getJMenuItem5() {
		if(jMenuItem5 == null) {
			jMenuItem5 = new JMenuItem();
			jMenuItem5.setText("Rearrange EntityInstance Positions");
			jMenuItem5.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent evt) {
					jMenuItem5ActionPerformed(evt);
				}
			});
		}
		return jMenuItem5;
	}
	
	private JSeparator getJSeparator5() {
		if(jSeparator5 == null) {
			jSeparator5 = new JSeparator();
		}
		return jSeparator5;
	}
	
	private void jMenuItem5ActionPerformed(ActionEvent evt) {
		System.out.println("jMenuItem5.actionPerformed, event="+evt);
		this.playground1.setReArrange(true);
		this.playground1.repaint();
	}

}
