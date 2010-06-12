#include "gtkInterface.h"
#include <iostream>
#include <fstream>
#include <sstream>

/**
 * Kontruktor, buduje interfejs, pobiera widgety, przypina sygnały
 *
 * @param argc
 * @param argv
 * @author cakper
 */
gtkInterface::gtkInterface(int *argc, char ***argv)
{
  this->test = 0;
  Gtk::Main kit(argc, argv);

  this->refBuilder = Gtk::Builder::create();
  this->refBuilder->add_from_file("interface.glade");

  this->refBuilder->get_widget("window1", this->widgetWindow);
  this->refBuilder->get_widget("post_parameters_entry", this->widgetPostParameters);
  this->refBuilder->get_widget("address_entry", this->widgetAddress);
  this->refBuilder->get_widget("time_spin", this->widgetTime);
  this->refBuilder->get_widget("requestes_spin", this->widgetRequests);
  this->refBuilder->get_widget("reset_button", this->widgetResetButton);
  this->refBuilder->get_widget("quit_button", this->widgetQuitButton);
  this->refBuilder->get_widget("run_button", this->widgetRunButton);
  this->refBuilder->get_widget("save_button", this->widgetSaveButton);
  this->refBuilder->get_widget("results_view", this->widgetResults);
  this->refBuilder->get_widget("scrolled_window", this->widgetScrolledWindow);
  this->refBuilder->get_widget("request_type_dropdown", this->widgetRequestType);

  if (this->widgetWindow == 0 ||
      this->widgetPostParameters == 0 ||
      this->widgetAddress == 0 ||
      this->widgetTime == 0 ||
      this->widgetRequests == 0 ||
      this->widgetResetButton == 0 ||
      this->widgetQuitButton == 0 ||
      this->widgetRunButton == 0 ||
      this->widgetSaveButton == 0 ||
      this->widgetResults == 0 ||
      this->widgetScrolledWindow == 0 ||
      this->widgetRequestType == 0)
    throw new Gtk::BuilderError(Gtk::BuilderError::UNHANDLED_TAG, "Nie znaleziono elementu w pliku interfejsu");

  this->widgetTypeComboboxModel = Gtk::ListStore::create(this->widgetColumn);
  this->widgetRequestType->set_model(this->widgetTypeComboboxModel);

  Gtk::TreeModel::Row row = *(this->widgetTypeComboboxModel->append());
  row[this->widgetColumn.widgetColumnType] = "GET";
  row = *(this->widgetTypeComboboxModel->append());
  row[this->widgetColumn.widgetColumnType] = "POST";

  this->setDefaults();

  this->widgetRequestType->pack_start(this->widgetColumn.widgetColumnType);

  this->widgetRequestType->signal_changed().connect(sigc::mem_fun(*this, &gtkInterface::widgetComboboxChanged));
  this->widgetResetButton->signal_clicked().connect(sigc::mem_fun(*this, &gtkInterface::setDefaults));
  this->widgetRunButton->signal_clicked().connect(sigc::mem_fun(*this, &gtkInterface::runTest));
  this->widgetSaveButton->signal_clicked().connect(sigc::mem_fun(*this, &gtkInterface::saveFile));
  this->widgetQuitButton->signal_clicked().connect(sigc::ptr_fun(gtk_main_quit));

  this->widgetResults->append_column("ID", this->widgetResultsModelColumns.resultId);
  this->widgetResults->append_column("Status", this->widgetResultsModelColumns.resultStatus);
  this->widgetResults->append_column("Czas", this->widgetResultsModelColumns.resultTime);

  Gtk::TreeView::Column * pColumn = this->widgetResults->get_column(0);
  pColumn->set_sort_column(this->widgetResultsModelColumns.resultId);

  pColumn = this->widgetResults->get_column(1);
  pColumn->set_sort_column(this->widgetResultsModelColumns.resultStatus);

  pColumn = this->widgetResults->get_column(2);
  pColumn->set_sort_column(this->widgetResultsModelColumns.resultTime);

  this->widgetScrolledWindow->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);

  kit.run(*this->widgetWindow);
}
/**
 * Destruktor, czyści pamięć
 *
 * @author cakper
 */
gtkInterface::~gtkInterface()
{
  if (this->test != 0)
    delete (this->test);
}

/**
 * Obsługuje sygnał zmiany wartości ComboBoxa
 *
 * @author cakper
 */
void gtkInterface::widgetComboboxChanged()
{

  Gtk::TreeModel::iterator iter = this->widgetRequestType->get_active();
  if (iter)
  {
    Gtk::TreeModel::Row row = *iter;
    if (row)
    {
      Glib::ustring type = row[this->widgetColumn.widgetColumnType];

      if ("GET" == type)
      {
        this->widgetPostParameters->set_sensitive(false);
      }
      else
      {
        this->widgetPostParameters->set_sensitive(true);
      }
    }
  }
}

/**
 * Resetuje wartości
 *
 * @author cakper
 */
void gtkInterface::setDefaults()
{
  this->widgetTime->set_value(1);
  this->widgetRequests->set_value(5);
  this->widgetAddress->set_text("http://localhost/");
  this->widgetRequestType->set_active(0);
  this->widgetPostParameters->set_sensitive(false);
  this->widgetRequestType->set_active(0);
  this->widgetPostParameters->set_sensitive(false);
  this->widgetResultsModel = Gtk::ListStore::create(this->widgetResultsModelColumns);
  this->widgetResultsModel->set_sort_column(this->widgetResultsModelColumns.resultId, Gtk::SORT_ASCENDING);
  this->widgetResults->set_model(this->widgetResultsModel);
  this->executed = false;

  if (this->test != 0)
    delete (this->test);

  this->test = 0;
}

/**
 * Zapisuje wynik testu do pliku
 *
 * @author cakper
 */
void gtkInterface::saveFile()
{
  if (this->executed)
  {
    Gtk::FileChooserDialog dialog("Wybierz plik do zapisu",
                                  Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*(this->widgetWindow));
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
    int ret = dialog.run();
    if (ret == Gtk::RESPONSE_OK || ret == Gtk::RESPONSE_ACCEPT)
    {
      std::string fileName = dialog.get_filename();
      std::ofstream file(fileName.c_str());

      file << this->test->getInfo() << std::endl;

      file << "Id;Status;Czas" << std::endl;

      for (long unsigned int i = 0; i < this->test->requests.size(); i++)
      {
        file << i + 1 << ";" << this->test->requests.at(i)->getStatus() << ";" << this->test->requests.at(i)->getRequestTime() << std::endl;
      }

      file.close();
    }
  }
  else
  {
    Gtk::MessageDialog dialog(*(this->widgetWindow), "Nie wykonałeś jeszcze testu!");
    dialog.run();
  }
}

/**
 * Wykonuje test, wyświetla dane w interfejsie
 *
 * @author cakper
 */
void gtkInterface::runTest()
{
  if (this->executed == false)
  {
    bool post;

    Gtk::TreeModel::iterator iter = this->widgetRequestType->get_active();
    if (iter)
    {
      Gtk::TreeModel::Row row = *iter;
      if (row)
      {
        Glib::ustring type = row[this->widgetColumn.widgetColumnType];

        if ("GET" == type)
        {
          post = false;
        }
        else
        {
          post = true;
        }
      }
    }

    try
    {

      this->test = new stressTest(this->widgetAddress->get_text(), (int) this->widgetRequests->get_value(), (int) this->widgetTime->get_value(), 0, post, this->widgetPostParameters->get_text());
      this->test->execute();

      this->executed = true;

      Gtk::TreeModel::Row row;
      for (long unsigned int i = 0; i < this->test->requests.size(); i++)
      {
        row = *(this->widgetResultsModel->append());
        row[this->widgetResultsModelColumns.resultId] = i + 1;
        row[this->widgetResultsModelColumns.resultStatus] = this->test->requests.at(i)->getStatus();
        row[this->widgetResultsModelColumns.resultTime] = this->test->requests.at(i)->getRequestTime();
      }
      std::ostringstream outputStats;
      outputStats << "Średni czas: " << "\t\t" << this->test->getAvgTime() << "s" << std::endl
        << "Mediana czasu: " << "\t\t" << this->test->getMedianTime() << "s" << std::endl
        << "% statusów 200: " << "\t" << this->test->getStatusPercentage() << "%";


      Gtk::MessageDialog dialog(*(this->widgetWindow), "Test został wykonany!");
      dialog.set_secondary_text(outputStats.str());

      dialog.run();
    }
    catch (std::string * e)
    {
      Gtk::MessageDialog dialog(*(this->widgetWindow), "Nie można wykonać testu!");
      dialog.set_secondary_text(e->c_str());
      dialog.run();
    }
  }
  else
  {
    Gtk::MessageDialog dialog(*(this->widgetWindow), "Nie można wykonać testu!");
    dialog.set_secondary_text("Najpierw zapisz i zresetuj wyniki poprzedniego.");

    dialog.run();
  }

}