/* Wrapper program that can be launched anywhere in Windows, and will then load a
   VehicleSim solver DLL. This example shows how a model can be extended with
   external C code (see external.c).

   Log:
   Apr 24, 10. M. Sayers. New function to load API: vs_get_api.
   May 18, 09. M. Sayers. New for CarSim 8.0. API install functions and vs_run.
*/

#include <windows.h> // Windows-specific header
#include <stdio.h>

#include "vs_deftypes.h" // VS types and definitions
#include "vs_api.h"      // VS API functions

#include "MessageManager/MessageManager.h"

#define ZCM_URL "udpm://239.255.76.67:7667?ttl=1"

void external_calc(vs_real t, vs_ext_loc where);
void external_echo(vs_ext_loc where);
vs_bool external_scan(char *, char *);
void external_setdef(void);

/* ---------------------------------------------------------------------------------
   Main program to run DLL with VS API.
--------------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
    MessageManager msg_manager(ZCM_URL);
    msg_manager.SubscribeAll();
    msg_manager.PublishAllAsync(200, 200);

    HMODULE vsDLL = NULL; // DLL with VS API
    char pathDLL[FILENAME_MAX], simfile[FILENAME_MAX] = {"simfile.sim"};

    // get simfile from argument list and load DLL
    if (argc > 1)
        strcpy(simfile, &argv[1][0]);
    if (vs_get_dll_path(simfile, pathDLL))
        return 1;
    vsDLL = LoadLibrary(pathDLL);

    // get API functions
    if (vs_get_api(vsDLL, pathDLL))
        return 1;

    // install external functions from custom code
    vs_install_setdef_function(external_setdef);
    vs_install_calc_function(external_calc);
    vs_install_echo_function(external_echo);
    vs_install_scan_function(external_scan);

    // Make the run; vs_run returns 0 if OK
    if (vs_run(simfile))
        MessageBox(NULL, vs_get_error_message(), NULL, MB_ICONERROR);

    // Wait for a keypress if the parameter opt_pause was specified.
    if (vs_opt_pause())
    {
        printf(
            "\n\nThe run ended normally. OPT_PAUSE was set to keep this display visible."
            "\nPress the Return key to exit this solver program. ");
        fgetc(stdin);
    }
    vs_free_library(vsDLL);

    return 0;
}

/* ---------------------------------------------------------------------------------
   Set up variables for the model extension. For the steering controller, define new
   units and parameters and set default values of the parameters that will be used
   if nothing is specified at run time.
---------------------------------------------------------------------------------- */
void external_setdef(void)
{
    // Add code here...
}

/* ---------------------------------------------------------------------------------
   Perform calculations involving the model extensions. This function is called from
   nine places as defined in vs_deftypes.h.
---------------------------------------------------------------------------------- */
void external_calc(vs_real t, vs_ext_loc where)
{
    switch (where)
    {

    // just after real Parsfile has been read
    case VS_EXT_AFTER_READ:
        // Add code here...
        break;

    // initialization after reading parsfile but before init
    case VS_EXT_EQ_PRE_INIT:
        // Add code here...
        break;

    case VS_EXT_EQ_INIT: // initialization after built-in init
        // Add code here...
        break;

    case VS_EXT_EQ_INIT2: // initialization after outputs are calculated
        // Add code here...
        break;

    case VS_EXT_EQ_IN: // calculate import variables at the start of a time step
        // Add code here...
        break;

    case VS_EXT_EQ_OUT: // calculate output variables at the end of a time step
                        // Add code here...
        break;

    case VS_EXT_EQ_SAVE: // save values for use in next time step
                         // Add code here...
        break;

    case VS_EXT_EQ_FULL_STEP: // calculate things only at the end of a full step
                              // Add code here...
        break;

    case VS_EXT_EQ_END: // calculations done at end of run
        // Add code here...
        break;
    }
}

/* ---------------------------------------------------------------------------------
   Write information into the current output echo file using the VS API function
   vs_write_to_echo_file. This function is called four times when generating the
   echo file as indicated with the argument where, which can have the values:
   VS_EXT_ECHO_TOP, VS_EXT_ECHO_SYPARS, VS_EXT_ECHO_PARS, and VS_EXT_ECHO_END
   (defined in vs_deftypes.h).
---------------------------------------------------------------------------------- */
void external_echo(vs_ext_loc where)
{
    switch (where)
    {
    case VS_EXT_ECHO_TOP: // top of echo file
        // Add code here...
        break;

    case VS_EXT_ECHO_SYPARS: // end of system parameter section
        // Add code here...
        break;

    case VS_EXT_ECHO_PARS: // end of model parameter section
        // Add code here...
        break;

    case VS_EXT_ECHO_END: // end of echo file
        // Add code here...
        break;
    }
}

/* ---------------------------------------------------------------------------------
   Scan a line read from the current input parsfile. Return TRUE if the keyword is
   recognized, FALSE if not. This is not needed for variables and parameters added
   using VS API funcitons, but might be helpful for some custom code.

   keyword -> string with current ALL CAPS keyword to be tested
   buffer  -> string with rest of line from parsfile.
--------------------------------------------------------------------------------- */
vs_bool external_scan(char *keyword, char *buffer)
{
    // Add code here to check the keyword and do something if recognized...
    return FALSE;
}
