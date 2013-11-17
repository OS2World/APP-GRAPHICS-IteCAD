# -------------------------------------------------
# Project created by QtCreator 2009-05-04T15:31:34
# -------------------------------------------------
QT += xml
TARGET = Itecad
TEMPLATE = app
SOURCES += main.cpp \
    win_main.cpp \
    widget_zoom.cpp \
    myGraphicsView.cpp \
    myGraphicsScene.cpp \
    myGraphicsItem.cpp \
    myComponent.cpp \
    utilities.cpp \
    undo_redo.cpp \
    win_edit_layers.cpp \
    widget_layers.cpp \
    layout_length.cpp \
    layout_size.cpp \
    layout_font.cpp \
    xml_sax_handler.cpp \
    xml_sax_generator.cpp \
    win_style.cpp \
    drc.cpp \
    win_object_prop.cpp \
    win_unicode_input.cpp \
    win_undo_redo_history.cpp \
    myUndoView.cpp \
    win_doc_manager.cpp \
    win_add_document.cpp \
    win_component_prop.cpp \
    win_add_component.cpp \
    preview.cpp \
    win_print.cpp \
    win_pdf_print_dialog.cpp \
    win_converter.cpp \
    win_drc.cpp \
    win_options.cpp \
    win_char_editor.cpp \
    win_xml_editor.cpp \
    char_editor.cpp \
    win_report.cpp \
    win_move_to.cpp \
    win_help_about.cpp \
    win_settings.cpp \
    win_page.cpp \
    layout_pad_via.cpp \
    win_ungated_pins.cpp \
    win_devices.cpp \
    xml_sax_handler_read_devices.cpp \
    win_gerber.cpp \
    win_drill.cpp \
    win_bill_of_materials.cpp \
    win_find_comp.cpp \
    win_find_text.cpp
HEADERS += win_main.h \
    tipi_dato.h \
    widget_zoom.h \
    myGraphicsScene.h \
    myGraphicsItem.h \
    myComponent.h \
    myGraphicsView.h \
    undo_redo.h \
    utilities.h \
    win_edit_layers.h \
    widget_layers.h \
    layout_length.h \
    layout_font.h \
    layout_size.h \
    xml_sax_generator.h \
    xml_sax_handler.h \
    win_style.h \
    drc.h \
    win_object_prop.h \
    win_unicode_input.h \
    win_undo_redo_history.h \
    myUndoView.h \
    win_doc_manager.h \
    win_add_document.h \
    win_component_prop.h \
    win_add_component.h \
    preview.h \
    win_print.h \
    win_pdf_print_dialog.h \
    win_converter.h \
    win_drc.h \
    win_options.h \
    win_char_editor.h \
    win_xml_editor.h \
    char_editor.h \
    win_report.h \
    win_move_to.h \
    win_help_about.h \
    win_settings.h \
    win_page.h \
    layout_pad_via.h \
    win_ungated_pins.h \
    win_devices.h \
    xml_sax_handler_read_devices.h \
    win_gerber.h \
    win_drill.h \
    win_bill_of_materials.h \
    win_find_comp.h \
    win_find_text.h
RESOURCES += Itecad.qrc
win32:RC_FILE = itecad.rc
os2:RC_FILE = itecad_os2.rc
