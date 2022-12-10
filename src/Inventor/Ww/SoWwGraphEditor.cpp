/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#if HAVE_CONFIG_H
#include <config.h>
#endif


#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/SoLists.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoWriteAction.h>

#include <sowwdefs.h>

#include <Inventor/Ww/SoWwGraphEditor.h>

// *************************************************************************

/*!
  \class SoWwGraphEditor Inventor/Ww/SoWwGraphEditor.h
  \brief The SoWwGraphEditor class is a component for viewing and editing
  the Inventor scene graph of a model.
  \ingroup editors

  This component is not implemented so far that editing is possible.
*/

/*!
  \enum SoWwGraphEditor::BuildFlag
  FIXME: write doc
*/

/*!
  \var SoWwGraphEditor::BuildFlag SoWwGraphEditor::MENUBAR
  FIXME: write doc
*/

/*!
  \var SoWwGraphEditor::BuildFlag SoWwGraphEditor::GRAPHEDITOR
  FIXME: write doc
*/

/*!
  \var SoWwGraphEditor::BuildFlag SoWwGraphEditor::STATUSBAR
  FIXME: write doc
*/

/*!
  \var SoWwGraphEditor::BuildFlag SoWwGraphEditor::EVERYTHING
  FIXME: write doc
*/

// *************************************************************************

SOWW_OBJECT_SOURCE(SoWwGraphEditor);

// *************************************************************************

/*!
  Public constructor.
*/

SoWwGraphEditor::SoWwGraphEditor(
  wxWindow * const parent,
  const char * const name,
  const SbBool embed,
  const int parts)
: inherited(parent, name, embed)
{
  this->constructor(TRUE, parts);
} // SoWwGraphEditor()

/*!
  Protected constructor.
*/

SoWwGraphEditor::SoWwGraphEditor(
  wxWindow * const parent,
  const char * const name,
  const SbBool embed,
  const int parts,
  const SbBool build)
: inherited(parent, name, embed)
{
  this->constructor(build, parts);
} // SoWwGraphEditor()

/*!
  Common constructor code.
*/

void
SoWwGraphEditor::constructor(
  const SbBool build,
  const int parts)
{
  this->scenegraph = (SoNode *) NULL;

  this->buildflags = parts & EVERYTHING;

  this->editorbase = (wxWindow *) NULL;
  this->menubar = (wxWindow *) NULL;
  this->grapheditor = (wxWindow *) NULL;
  this->graphroot = (wxWindow *) NULL;
  this->statusbar = (wxWindow *) NULL;
  this->statusmessage = (wxWindow *) NULL;

  this->horizontal = (WwAdjustment *) NULL;
  this->vertical = (WwAdjustment *) NULL;

  this->setClassName("SoWwGraphEditor");

  if (! build) return;
  this->setSize(SbVec2s(350, 450));
  wxWindow * parent = this->getParentWidget();
  wxWindow * editor = this->buildWidget(parent);
  this->setBaseWidget(editor);
} // constructor()

/*!
  Destructor.
*/

SoWwGraphEditor::~SoWwGraphEditor(
  void)
{
} // ~SoWwGraphEditor()

// *************************************************************************

/*!
  Set the scene graph to edit.
*/

void
SoWwGraphEditor::setSceneGraph(// virtual
  SoNode * root)
{
  if (this->scenegraph) {
    this->clearSceneGraphTree();
    this->scenegraph->unref();
  }
  this->scenegraph = root;
  if (this->scenegraph) {
    this->scenegraph->ref();
    this->buildSceneGraphTree();
  }
} // setSceneGraph()

/*!
  Returns the scene graph used in the graph editor.
*/

SoNode *
SoWwGraphEditor::getSceneGraph(
  void) const
{
  return this->scenegraph;
} // getSceneGraph;

/*
  FIXME: write doc
*/

wxWindow *
SoWwGraphEditor::buildSubGraph(
  wxWindow * parent,
  SoNode * node)
{
  assert(node != NULL);
  const char * classname = node->getTypeId().getName().getString();
  SbName nodename = node->getName();
  wxWindow * root;
  if (! nodename) {
    root = WW_WIDGET(ww_tree_item_new_with_label((gchar*)classname));
  } else {
    const char * name = nodename.getString();
    char buf[128];
    sprintf(buf, "%s \"%s\"", classname, name);
    root = WW_WIDGET(ww_tree_item_new_with_label(buf));
  }
  ww_object_set_data(WW_OBJECT(root), "SoNode", (gpointer) node);
  ww_widget_show(root);
  ww_tree_append(WW_TREE(parent), root);
  ww_signal_connect(WW_OBJECT(root), "select",
    WW_SIGNAL_FUNC(SoWwGraphEditor::selectionCB), (gpointer) this);

  wxWindow * substuff = WW_WIDGET(ww_tree_new());
  ww_tree_item_set_subtree(WW_TREE_ITEM(root), substuff);

  SoFieldList fields;
  if (node->getFields(fields) > 0) {
    wxWindow * fieldsitem = WW_WIDGET(ww_tree_item_new_with_label("[fields]"));
    ww_widget_show(fieldsitem);
    ww_tree_append(WW_TREE(substuff), fieldsitem);
    wxWindow * fieldstree = WW_WIDGET(ww_tree_new());
    ww_tree_item_set_subtree(WW_TREE_ITEM(fieldsitem), fieldstree);
    for (int i = 0; i < fields.getLength(); i++) {
      SoField * field = fields[i];
      SbName fieldname;
      node->getFieldName(field, fieldname);
      const char * fieldnamestr = fieldname.getString();
      wxWindow * fieldwidget = WW_WIDGET(ww_tree_item_new_with_label((gchar*)fieldnamestr));
      ww_object_set_data(WW_OBJECT(fieldwidget), "SoNode", (gpointer) node);
      ww_object_set_data(WW_OBJECT(fieldwidget), "SoField", (gpointer) field);
      ww_widget_show(fieldwidget);
      ww_tree_append(WW_TREE(fieldstree), fieldwidget);
      ww_signal_connect(WW_OBJECT(fieldwidget), "select",
        WW_SIGNAL_FUNC(SoWwGraphEditor::selectionCB), (gpointer) this);
    }
  }

  SoChildList * children = node->getChildren();
  if (children != NULL && children->getLength() > 0) {
    for (int i = 0; i < children->getLength(); i++)
      wxWindow * kid = SoWwGraphEditor::buildSubGraph(substuff, (SoNode *) children->get(i));
  }
  return root;
}

/*!
  FIXME: write doc
*/

void
SoWwGraphEditor::buildSceneGraphTree(// virtual, protected
  void)
{
  wxWindow * graph = this->buildSubGraph(this->graphroot, this->scenegraph);
  ww_tree_item_expand(WW_TREE_ITEM(graph));
} // buildSceneGraphTree()

/*!
  FIXME: write doc
*/

void
SoWwGraphEditor::clearSceneGraphTree(// virtual, protected
  void)
{
  SoDebugError::postInfo("SoWwGraphEditor::clearSceneGraphTree", 
    _("[not implemented]"));
} // clearSceneGraphTree()

/*!
  FIXME: write doc
*/

void
SoWwGraphEditor::saveSceneGraph(// virtual, protected
  void)
{
  if (! this->scenegraph) {
    this->setStatusMessage(_("No scene to save."));
    return;
  }
  SoOutput * output = new SoOutput;
  if (! output->openFile("scene.iv")) {
    this->setStatusMessage(_("Error opening 'scene.iv' for writing."));
    delete output;
    return;
  }
  SoWriteAction writer(output);
  writer.apply(this->scenegraph);
  delete output;
  this->setStatusMessage(_("Scene saved in 'scene.iv'."));
} // saveSceneGraph()

// *************************************************************************

/*!
  This method returns builds and returns the basic graph editor component.
*/

wxWindow *
SoWwGraphEditor::buildWidget(// virtual, protected
  wxWindow * parent)
{
  this->editorbase = WW_WIDGET(ww_vbox_new(FALSE, 0));
  if (this->buildflags & MENUBAR) {
    this->menubar = this->buildMenuBarWidget(this->editorbase);
    ww_widget_set_usize(WW_WIDGET(this->menubar), 0, 30);
    ww_widget_show(this->menubar);
    ww_box_pack_start(WW_BOX(this->editorbase), WW_WIDGET(this->menubar), FALSE, TRUE, 0);
  }
  if (this->buildflags & GRAPHEDITOR) {
    this->grapheditor = this->buildGraphEditorWidget(this->editorbase);
    ww_widget_show(this->grapheditor);
    ww_box_pack_start(WW_BOX(this->editorbase), WW_WIDGET(this->grapheditor), TRUE, TRUE, 0);
  }
  if (this->buildflags & STATUSBAR) {
    this->statusbar = this->buildStatusBarWidget(this->editorbase);
    ww_widget_set_usize(WW_WIDGET(this->statusbar), 0, 30);
    ww_widget_show(this->statusbar);
    ww_box_pack_end(WW_BOX(this->editorbase), WW_WIDGET(this->statusbar), FALSE, TRUE, 0);
  }
  return this->editorbase;
} // buildWidget()

/*!
  This function builds and returns the graph editor menu bar.
*/

wxWindow *
SoWwGraphEditor::buildMenuBarWidget(// virtual, protected
  wxWindow * parent)
{
  wxWindow * menubar = WW_WIDGET(ww_menu_bar_new());
  wxWindow * filemenuitem = ww_menu_item_new_with_label(_("File"));
  ww_widget_show(filemenuitem);
  ww_menu_bar_append(WW_MENU_BAR(menubar), WW_WIDGET(filemenuitem));
  wxWindow * filemenu = WW_WIDGET(ww_menu_new());
  ww_widget_show(filemenu);
  wxWindow * saveitem = ww_menu_item_new_with_label(_("Save"));
  ww_widget_show(saveitem);
  ww_menu_append(WW_MENU(filemenu), WW_WIDGET(saveitem));
  ww_signal_connect(WW_OBJECT(saveitem), "activate",
    WW_SIGNAL_FUNC(SoWwGraphEditor::saveCB), (gpointer) this);
  wxWindow * closeitem = ww_menu_item_new_with_label(_("Close"));
  ww_widget_show(closeitem);
  ww_menu_append(WW_MENU(filemenu), WW_WIDGET(closeitem));
  ww_signal_connect(WW_OBJECT(closeitem), "activate",
    WW_SIGNAL_FUNC(SoWwGraphEditor::closeCB), (gpointer) this);
  ww_menu_item_set_submenu(WW_MENU_ITEM(filemenuitem), WW_WIDGET(filemenu));
  return menubar;
} // buildMenuBarWidget()

/*!
  This function builds and returns the actual graph editor widget.
*/

wxWindow *
SoWwGraphEditor::buildGraphEditorWidget(// virtual, protected
  wxWindow * parent)
{
  /* value, lower, upper, step_increment, page_increment, page_size */
  this->vertical = WW_ADJUSTMENT(ww_adjustment_new(0, 0, 1000, 10, 100, 100));
  this->horizontal = WW_ADJUSTMENT(ww_adjustment_new(0, 0, 1000, 10, 100, 1000));

  wxWindow * editor = WW_WIDGET(ww_scrolled_window_new(horizontal, vertical));
  ww_scrolled_window_set_policy(
    WW_SCROLLED_WINDOW(editor), WW_POLICY_NEVER, WW_POLICY_ALWAYS);

  this->graphroot = WW_WIDGET(ww_tree_new());
  ww_widget_show(this->graphroot);
  ww_scrolled_window_add_with_viewport(WW_SCROLLED_WINDOW(editor), this->graphroot);

  return editor;
} // buildGraphEditorWidget()

/*!
  This function builds and returns the graph editor status bar.
*/

wxWindow *
SoWwGraphEditor::buildStatusBarWidget(// virtual, protected
  wxWindow * parent)
{
  wxWindow * statusframe = WW_WIDGET(ww_frame_new((gchar *) NULL));
  ww_widget_set_usize(statusframe, 0, 30);
  ww_frame_set_shadow_type(WW_FRAME(statusframe), WW_SHADOW_ETCHED_IN);
  ww_container_set_border_width(WW_CONTAINER(statusframe), 3);
  
  wxWindow * statuswidgets = ww_hbox_new(FALSE, 0);
  ww_widget_set_usize(statusframe, 0, 30);
  ww_container_add(WW_CONTAINER(statusframe), statuswidgets);
  ww_widget_show(statuswidgets);

  this->statusmessage = WW_WIDGET(ww_label_new((gchar *) NULL));
  ww_label_set_justify(WW_LABEL(this->statusmessage), WW_JUSTIFY_LEFT);
  ww_widget_show(this->statusmessage);
  ww_box_pack_start(WW_BOX(statuswidgets), this->statusmessage, FALSE, TRUE, 3);

  return statusframe;
} // buildStatusBarWidget()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoWwGraphEditor::nodeSelection(// virtual, protected
  wxWindow * treeitem,
  SoNode * node)
{
  this->setStatusMessage(node->getTypeId().getName().getString());
} // nodeSelection()

/*!
  FIXME: write doc
*/

void
SoWwGraphEditor::fieldSelection(// virtual, protected
  wxWindow * treeitem,
  SoNode * node,
  SoField * field)
{
  this->setStatusMessage(field->getTypeId().getName().getString());
} // fieldSelection()

// *************************************************************************

// doc in super
void
SoWwGraphEditor::sizeChanged(const SbVec2s & size)
{
  // inherited::sizeChanged(size);
} // sizeChanged()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoWwGraphEditor::saveCB(// static, private
  WwObject * obj,
  gpointer closure)
{
  assert(closure != NULL);
  SoWwGraphEditor * editor = (SoWwGraphEditor *) closure;
  editor->saveSceneGraph();
} // saveCB()

/*!
  FIXME: write doc
*/

void
SoWwGraphEditor::closeCB(// static, private
  WwObject * obj,
  gpointer closure)
{
  assert(closure != NULL);
  SoWwGraphEditor * editor = (SoWwGraphEditor *) closure;
  editor->hide();
} // closeCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoWwGraphEditor::setStatusMessage(// virtual, protected
  const char * message)
{
  ww_label_set_text(WW_LABEL(this->statusmessage), message);
} // setStatusMessage()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoWwGraphEditor::selectionCB(
  WwObject * object,
  gpointer closure)
{
  assert(closure != NULL);
  SoWwGraphEditor * editor = (SoWwGraphEditor *) closure;
  SoNode * node = (SoNode *) ww_object_get_data(WW_OBJECT(object), "SoNode");
  SoField * field = (SoField *) ww_object_get_data(WW_OBJECT(object), "SoField");
  if (node && ! field)
    editor->nodeSelection(WW_WIDGET(object), node);
  else if (node && field)
    editor->fieldSelection(WW_WIDGET(object), node, field);
  else
    SoDebugError::postInfo("SoWwGraphEditor::selectionCB", "[invalid selection data]");
} // selectionCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoWwGraphEditor::getDefaultWidgetName(// virtual, protected
  void) const
{
  static const char defaultWidgetName[] = "SoWwGraphEditor";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc
*/

const char *
SoWwGraphEditor::getDefaultTitle(// virtual, protected
  void) const
{
  static const char defaultTitle[] = N_("Graph Editor");
  return _(defaultTitle);
} // getDefaultTitle()

/*!
  FIXME: write doc
*/

const char *
SoWwGraphEditor::getDefaultIconTitle(// virtual, protected
  void) const
{
  static const char defaultIconTitle[] = N_("Graph Editor");
  return _(defaultIconTitle);
} // getDefaultIconTitle()

// *************************************************************************
