/* 
 * Klasa interfejsu GTK
 */
/**
 * Klasa interfejsu użytkownika
 */
#ifndef _GTKINTERFACE_H
#define	_GTKINTERFACE_H

#include <gtkmm-2.4/gtkmm.h>

#include "stressTest.h"

class gtkInterface {
public:
    gtkInterface(int *argc, char ***argv);
    virtual ~gtkInterface();
private:\
    void runButtonClicked();
    stressTest * test;
    bool executed;
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error;
    Glib::RefPtr<Gtk::Builder> refBuilder;
    void setDefaults();
    void saveFile();
    void runTest();
    void widgetComboboxChanged();
    class comboboxModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        comboboxModelColumns() {
            add(widgetColumnType);
        }
        Gtk::TreeModelColumn<Glib::ustring> widgetColumnType;
    };
    comboboxModelColumns widgetColumn;
    class resultsModelColumns : public Gtk::TreeModelSort::ColumnRecord {
    public:
        resultsModelColumns() {
            add(resultId);
            add(resultStatus);
            add(resultTime);
        }
        Gtk::TreeModelColumn<unsigned int> resultId;
        Gtk::TreeModelColumn<unsigned int> resultStatus;
        Gtk::TreeModelColumn<double> resultTime;
    };
    resultsModelColumns widgetResultsModelColumns;
    Glib::RefPtr<Gtk::ListStore> widgetResultsModel;
    Gtk::TreeView * widgetResults;
    Gtk::Entry * widgetPostParameters;
    Gtk::Entry * widgetAddress;
    Gtk::SpinButton * widgetTime;
    Gtk::SpinButton * widgetRequests;
    Gtk::ComboBox * widgetRequestType;
    Gtk::Button * widgetQuitButton;
    Gtk::Button * widgetRunButton;
    Gtk::Button * widgetResetButton;
    Gtk::Button * widgetSaveButton;
    Glib::RefPtr<Gtk::ListStore> widgetTypeComboboxModel;
    Gtk::ScrolledWindow * widgetScrolledWindow;

    Gtk::Window * widgetWindow;
};

#endif	/* _GTKINTERFACE_H */

