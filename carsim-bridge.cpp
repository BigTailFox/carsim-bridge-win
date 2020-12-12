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
#define DEBUG_PRINT

/* ---------------------------------------------------------------------------------
   Function Prototypes, Variables
--------------------------------------------------------------------------------- */
void external_calc(vs_real t, vs_ext_loc where);
void external_echo(vs_ext_loc where);
vs_bool external_scan(char *, char *);
void external_setdef(void);

MessageManager msg_manager(ZCM_URL);

static vs_real
    //input
    *STEER,
    *THROTTLE,
    *BRAKE,
    *ZL1, *ZL2, *ZR1, *ZR2,
    *DZDXL1, *DZDXL2, *DZDXR1, *DZDXR2,
    *DZDYL1, *DZDYL2, *DZDYR1, *DZDYR2,
    *MUXL1, *MUXL2, *MUXR1, *MUXR2,
    *MUYL1, *MUYL2, *MUYR1, *MUYR2,
    //output
    *X, *Y, *Z,
    *VX, *VY, *VZ,
    *DVX, *DVY, *DVZ,
    *YAW, *PITCH, *ROLL,
    *AVY, *AVP, *AVR,
    *DAVY, *DAVP, *DAVR,
    *XL1, *XL2, *XR1, *XR2, *YL1, *YL2, *YR1, *YR2,
    *TSTAMP;

/* ---------------------------------------------------------------------------------
   Main program to run DLL with VS API.
--------------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
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
    // output: simulation timestamp
    TSTAMP = vs_get_var_ptr("T_STAMP");

    // input: control
    STEER = vs_get_var_ptr("IMP_STEER_SW");           // steerwheel angle (deg)
    THROTTLE = vs_get_var_ptr("IMP_THROTTLE_ENGINE"); // open loop throttle control (-)
    BRAKE = vs_get_var_ptr("IMP_FBK_PDL");            // brake pedal force (N)

    // input: tire contact ground Z
    ZL1 = vs_get_var_ptr("IMP_ZGND_L1"); // Z coordinate of ground at the tire CTC (m)
    ZL2 = vs_get_var_ptr("IMP_ZGND_L2");
    ZR1 = vs_get_var_ptr("IMP_ZGND_R1");
    ZR2 = vs_get_var_ptr("IMP_ZGND_R2");

    // input: tire contact friction
    MUXL1 = vs_get_var_ptr("IMP_MUX_L1"); // Ground longitudinal friction at the tire CTC (-)
    MUXL2 = vs_get_var_ptr("IMP_MUX_L2");
    MUXR1 = vs_get_var_ptr("IMP_MUX_R1");
    MUXR2 = vs_get_var_ptr("IMP_MUX_R2");
    MUYL1 = vs_get_var_ptr("IMP_MUY_L1"); // Ground lateral friction at the tire CTC (-)
    MUYL2 = vs_get_var_ptr("IMP_MUY_L2");
    MUYR1 = vs_get_var_ptr("IMP_MUY_R1");
    MUYR2 = vs_get_var_ptr("IMP_MUY_R2");

    // input: tire contact slop
    DZDXL1 = vs_get_var_ptr("IMP_DZDX_L1"); // slope DZ/DX at L1 (-)
    DZDXL2 = vs_get_var_ptr("IMP_DZDX_L2");
    DZDXR1 = vs_get_var_ptr("IMP_DZDX_R1");
    DZDXR2 = vs_get_var_ptr("IMP_DZDX_R2");
    DZDYL1 = vs_get_var_ptr("IMP_DZDY_L1");
    DZDYL2 = vs_get_var_ptr("IMP_DZDY_L2");
    DZDYR1 = vs_get_var_ptr("IMP_DZDY_R1");
    DZDYR2 = vs_get_var_ptr("IMP_DZDY_R2");

    // output: vehicle transform
    X = vs_get_var_ptr("XO"); // inertial, x coord. (m)
    Y = vs_get_var_ptr("YO");
    Z = vs_get_var_ptr("ZO");
    YAW = vs_get_var_ptr("YAW");
    PITCH = vs_get_var_ptr("PITCH");
    //ROLL = vs_get_var_ptr("ROLL"); // the angle between [sy] and the horizontal plane normal to [nz], NOT EULER ANGLE (deg)
    ROLL = vs_get_var_ptr("ROLL_E"); // euler roll angle (deg)

    // output: velocity and angular velocity
    VX = vs_get_var_ptr("VXNF_SM"); // the [nx] component of the velocity vector of the sprung mass CG (km/h)
    VY = vs_get_var_ptr("VYNF_SM"); // the [ny] component of the velocity vector of the sprung mass CG (km/h)
    VZ = vs_get_var_ptr("VZ_SM");   // the [nz] component of the velocity vector of the sprung mass CG (km/h)
    AVP = vs_get_var_ptr("AV_P");   // the time derivative of the Euler pitch angle of the sprung mass (deg/s)
    AVR = vs_get_var_ptr("AV_R");
    AVY = vs_get_var_ptr("AV_Y");
    // VX = vs_get_var_ptr("VX");    // the speed of the instant CG of the vehicle unit in the ISO/SAE [x] direction (km/h)
    // VY = vs_get_var_ptr("VY");    // the speed of the instant CG of the vehicle unit in the ISO/SAE [y] direction (km/h)
    // VZ = vs_get_var_ptr("VZ");    // the speed of the instant CG of the vehicle unit in the [nz] direction (km/h)

    // output: acceleration and angular acceleration
    DVX = vs_get_var_ptr("AX_SM"); // the [x] component of the acceleration vector of the sprung mass CG (g)
    DVY = vs_get_var_ptr("AX_SM");
    DVZ = vs_get_var_ptr("AZ_SM");
    DAVP = vs_get_var_ptr("AA_P"); // the second time derivative of the Euler pitch angle of the sprung mass (rad/s2)
    DAVR = vs_get_var_ptr("AA_R");
    DAVY = vs_get_var_ptr("AA_Y");
    // DVX = vs_get_var_ptr("Ax"); // the acceleration of the instant CG of the vehicle unit in the ISO/SAE [x] direction (g)
    // DVY = vs_get_var_ptr("Ay");
    // DVZ = vs_get_var_ptr("Az"); // the acceleration of the instant CG of the vehicle unit, in the [nz] direction (g)

    // output: tire contact ground X,Y
    XL1 = vs_get_var_ptr("XCTC_L1");
    XL2 = vs_get_var_ptr("XCTC_L2");
    XR1 = vs_get_var_ptr("XCTC_R1");
    XR2 = vs_get_var_ptr("XCTC_R2");
    YL1 = vs_get_var_ptr("YCTC_L1");
    YL2 = vs_get_var_ptr("YCTC_L2");
    YR1 = vs_get_var_ptr("YCTC_R1");
    YR2 = vs_get_var_ptr("YCTC_R2");
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

#ifdef DEBUG_PRINT
        printf("\nVS_EXT_AFTER_READ: activate import variables\n");
#endif

        vs_statement("IMPORT", "IMP_STEER_SW vs_replace", 1);
        vs_statement("IMPORT", "IMP_THROTTLE_ENGINE vs_replace", 1);
        vs_statement("IMPORT", "IMP_FBK_PDL vs_replace", 1);

        vs_statement("IMPORT", "IMP_ZGND_L1 vs_replace", 1);
        vs_statement("IMPORT", "IMP_ZGND_L2 vs_replace", 1);
        vs_statement("IMPORT", "IMP_ZGND_R1 vs_replace", 1);
        vs_statement("IMPORT", "IMP_ZGND_R2 vs_replace", 1);

        vs_statement("IMPORT", "IMP_MUX_L1 vs_replace", 1);
        vs_statement("IMPORT", "IMP_MUX_L2 vs_replace", 1);
        vs_statement("IMPORT", "IMP_MUX_R1 vs_replace", 1);
        vs_statement("IMPORT", "IMP_MUX_R2 vs_replace", 1);
        vs_statement("IMPORT", "IMP_MUY_L1 vs_replace", 1);
        vs_statement("IMPORT", "IMP_MUY_L2 vs_replace", 1);
        vs_statement("IMPORT", "IMP_MUY_R1 vs_replace", 1);
        vs_statement("IMPORT", "IMP_MUY_R2 vs_replace", 1);

        vs_statement("IMPORT", "IMP_DZDX_L1 vs_replace", 1);
        vs_statement("IMPORT", "IMP_DZDX_L2 vs_replace", 1);
        vs_statement("IMPORT", "IMP_DZDX_R1 vs_replace", 1);
        vs_statement("IMPORT", "IMP_DZDX_R2 vs_replace", 1);
        vs_statement("IMPORT", "IMP_DZDY_L1 vs_replace", 1);
        vs_statement("IMPORT", "IMP_DZDY_L2 vs_replace", 1);
        vs_statement("IMPORT", "IMP_DZDY_R1 vs_replace", 1);
        vs_statement("IMPORT", "IMP_DZDY_R2 vs_replace", 1);
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

#ifdef DEBUG_PRINT
        printf("\nVS_EXT_EQ_IN: update import variables\n");
#endif

        if (msg_manager.carsim_control_.valid)
        {

#ifdef DEBUG_PRINT
            printf("           : update control\n");
#endif

            *THROTTLE = msg_manager.carsim_control_.throttle;
            *BRAKE = msg_manager.carsim_control_.brake;
            *STEER = msg_manager.carsim_control_.steer;
        }
        if (msg_manager.road_contact_.valid)
        {

#ifdef DEBUG_PRINT
            printf("           : update road contact point\n");
#endif

            *ZL1 = msg_manager.road_contact_.left_front.z;
            *ZL2 = msg_manager.road_contact_.left_rear.z;
            *ZR1 = msg_manager.road_contact_.right_front.z;
            *ZR2 = msg_manager.road_contact_.right_rear.z;

            *MUXL1 = msg_manager.road_contact_.left_front.friction;
            *MUYL1 = msg_manager.road_contact_.left_front.friction;
            *MUXL2 = msg_manager.road_contact_.left_rear.friction;
            *MUYL2 = msg_manager.road_contact_.left_rear.friction;
            *MUXR1 = msg_manager.road_contact_.right_front.friction;
            *MUYR1 = msg_manager.road_contact_.right_front.friction;
            *MUXR2 = msg_manager.road_contact_.right_rear.friction;
            *MUYR2 = msg_manager.road_contact_.right_rear.friction;
        }

        break;

    case VS_EXT_EQ_OUT: // calculate output variables at the end of a time step

#ifdef DEBUG_PRINT
        printf("\nVS_EXT_EQ_OUT: update output variables\n");
#endif

        msg_manager.carsim_state_.valid = 1;

        msg_manager.carsim_state_.x = *X;
        msg_manager.carsim_state_.y = *Y;
        msg_manager.carsim_state_.z = *Z;
        msg_manager.carsim_state_.yaw = *YAW;
        msg_manager.carsim_state_.roll = *ROLL;
        msg_manager.carsim_state_.pitch = *PITCH;

        msg_manager.carsim_state_.vx = *VX;
        msg_manager.carsim_state_.vy = *VY;
        msg_manager.carsim_state_.vz = *VZ;
        msg_manager.carsim_state_.avy = *AVY;
        msg_manager.carsim_state_.avp = *AVP;
        msg_manager.carsim_state_.avr = *AVR;

        msg_manager.carsim_state_.dvx = *DVX;
        msg_manager.carsim_state_.dvy = *DVY;
        msg_manager.carsim_state_.dvz = *DVZ;
        msg_manager.carsim_state_.davy = *DAVY;
        msg_manager.carsim_state_.davp = *DAVP;
        msg_manager.carsim_state_.davr = *DAVR;

#ifdef DEBUG_PRINT
        printf("             : update vehicle state\n");
#endif

        msg_manager.road_query_.valid = 1;

        msg_manager.road_query_.left_front.x = *XL1;
        msg_manager.road_query_.left_front.y = *YL1;
        msg_manager.road_query_.left_rear.x = *XL2;
        msg_manager.road_query_.left_rear.y = *YL2;
        msg_manager.road_query_.right_front.x = *XR1;
        msg_manager.road_query_.right_front.y = *YR1;
        msg_manager.road_query_.right_rear.x = *XR2;
        msg_manager.road_query_.right_rear.y = *YR2;

#ifdef DEBUG_PRINT
        printf("             : update road query point\n");
#endif

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
