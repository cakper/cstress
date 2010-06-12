/**
 *  @author Kacper "Cakper" Gunia <cakper@gmail.com>
 */
#include "gtkInterface.h"
#include "gtkmm-2.4/gtkmm.h"

int main(int argc, char *argv[])
{

  try
  {
    gtkInterface interface(&argc, &argv);
  }
  catch (const Glib::FileError& ex)
  {
    Gtk::MessageDialog dialog("Błąd pliku interfejsu!");
    dialog.set_secondary_text(ex.what());
    dialog.run();
  }
  catch (const Gtk::BuilderError* ex)
  {
    Gtk::MessageDialog dialog("Błąd budowniczego interfejsu!");
    dialog.set_secondary_text(ex->what());
    dialog.run();
  }

  return ( 0);
}