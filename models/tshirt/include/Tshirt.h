
#ifndef TSHIRT_H
#define TSHIRT_H

#include "kernels/schaake_partitioning.hpp"
#include <cmath>
#include <vector>

namespace tshirt {

    //! Tshirt parameters struct
    /*!
        This structure provides storage for the parameters of the Tshirt hydrological model
    */

    struct tshirt_params
    {
        double maxsmc;              //!< saturated soil moisture content (sometimes theta_e)
        double wltsmc;              //!< wilting point soil moisture content
        double satdk;               //!< saturated hydraulic conductivity [m s^-1]
        double satpsi;              //!< saturated capillary head [m]
        // TODO: explain more what this is
        double slope;               //!< SLOPE parameter
        double b;                   //!< 'b' exponent on Clapp-Hornberger soil water relations (sometime bexp)
        double multiplier;          //!< the multiplier applied to 'satdk' to route water rapidly downslope in subsurface
        double alpha_fc;            //!< alpha constant for given soil type for relative suction head value, with respect to Hatm
        double Klf;                 //!< lateral flow independent calibration parameter
        double Kn;                  //!< Nash cascade linear reservoir coefficient lateral flow parameter
        double Cgw;                 //!< Ground water flow param
        double expon;               //!< Ground water flow exponent param (analogous to NWM 2.0 expon param)
        double Sgwmax;              //!< Ground water flow max storage param (analogous to NWM 2.0 zmax param)
        const double depth = 2.0;         //!< Total soil column depth ('D') [m]

        //! Constructor for tshirt parameters
        /*!
            Constructor for tshirt param objects.
        */
        tshirt_params(double maxsmc, double wltsmc, double satdk, double satpsi, double slope, double b,
                double multiplier, double alpha_fc, double Klf, double Kn, double Cgw, double expon) :
                maxsmc(maxsmc),
                wltsmc(wltsmc),
                satdk(satdk),
                satpsi(satpsi),
                slope(slope),
                b(b),
                multiplier(multiplier),
                alpha_fc(alpha_fc),
                Klf(Klf),
                Kn(Kn),
                Cgw(Cgw),
                expon(expon)
        {

        }

    };

    /*!
     * Tshirt state structure
     *
     * This structure provides storage for the state used by Tshirt hydrological model at a particular time step.
     */
    struct tshirt_state
    {
        // TODO: confirm this is correct
        double Ss;           //!< current water storage in soil column nonlinear reservoir
        double Sgw;          //!< current water storage in ground water nonlinear reservoir

        // I think this doesn't belong in state, and so is just in run() below
        //double column_total_soil_moisture_deficit;    //!< soil column total moisture deficit

        tshirt_state(double ss, double sgw) : Ss(ss), Sgw(sgw) {}
    };

    /*!
     * Tshirt flux structure
     *
     * This structure provides storage for the fluxes generated by Tshirt at any time step
     */
    struct tshirt_fluxes
    {
        double surface_runoff;  //!< Direct surface runoff
        double Qgw;             //!< Groundwater flow from groundwater reservoir to channel flow
        double Qperc;           //!< Percolation flow from subsurface to groundwater reservoir
        double Qlf;             //!< Lateral subsurface flow

        tshirt_fluxes(double q_gw, double q_perc, double q_lf, double runoff ) : Qgw(q_gw), Qperc(q_perc), Qlf(q_lf), surface_runoff(runoff)
        {

        }
    };

    // TODO: consider combining with or differentiating from similar hymod enum
    enum TshirtErrorCodes
    {
        TSHIRT_NO_ERROR = 0,
        TSHIRT_MASS_BALANCE_ERROR = 100
    };

    /*!
     * Tshirt kernel class
     *
     * This class implements the Tshirt hydrological model.
     */
    class tshirt_kernel
    {
    public:

        //! stub function to simulate losses due to evapotransportation
        static double calc_et(double soil_m, void* et_params)
        {
            // TODO: implement
            return 0.0;
        }

        //! Calculate the max water storage for soil based on the given Tshirt parameters
        static double calc_Ssmax(const tshirt_params& params)
        {
            return params.depth * params.maxsmc;
        }

        //! Calculate the Cschaake, or the Schaake adjusted magic constant by soil type, based on the given Tshirt parameters
        static double calc_Cschaake(const tshirt_params& params)
        {
            return 3.0 * params.satdk / (2.0e-6);
        }

        /*!
         * Calculate the height above water table based on the given Tshirt parameters
         *
         * Calculate the height above water table based on known constants and the particular `alpha` value given for
         * relative soil suction head, as provided in the Tshirt parameters.
         *
         * @param params
         * @return
         */
        static double calc_Hwt(const tshirt_params& params)
        {
            // H_wt = alpha_fc * H_atm ; H_atm = P_atm / gamma ; P_atm = 101,300 [Pa] ; gamma = 9,810 [N m^-3] ('very nearly')
            return params.alpha_fc * (101300 / 9810);
        }

        static double calc_Sfc(const tshirt_params& params, const tshirt_state& state)
        {
            // TODO: account for possiblity of Hwt being less than 0.5 (though initially, it looks like this will never be the case)
            double z1 = calc_Hwt(params) - 0.5;
            double z2 = z1 + 2;

            // Note that z^( 1 - (1/b) ) / (1 - (1/b)) == b * (z^( (b - 1) / b ) / (b - 1)
            return params.maxsmc * pow((1.0 / params.satpsi), (-1.0 / params.b)) *
                   ((params.b * pow(z2, ((params.b - 1) / params.b)) / (params.b - 1)) -
                    (params.b * pow(z1, ((params.b - 1) / params.b)) / (params.b - 1)));
        }

        //! run one time step of tshirt
        static int run(
                double dt,
                tshirt_params params,        //!< static parameters for tshirt
                tshirt_state state,          //!< model state
                tshirt_state& new_state,     //!< model state struct to hold new model state
                tshirt_fluxes& fluxes,       //!< model flux object to hold calculated fluxes
                double input_flux_meters,          //!< the amount water entering the system this time step
                void* et_params)            //!< parameters for the et function
        {
            double Ssmax = calc_Ssmax(params);

            double column_total_soil_moisture_deficit = Ssmax - state.Ss;

            double surface_runoff, subsurface_infiltration_flux;

            Schaake_partitioning_scheme(dt, calc_Cschaake(params), column_total_soil_moisture_deficit, input_flux_meters,
                    &surface_runoff, &subsurface_infiltration_flux);

            // TODO: properly handle GIUH surface runoff

            state.Ss += subsurface_infiltration_flux;

            double Sfc = calc_Sfc(params, state);

            // lateral subsurface flow
            double Qlf = params.Klf * (state.Ss - Sfc) / (Ssmax - Sfc);

            // TODO: account for Nash Cascade

            // default percolation flow to 0
            double Qperc = 0;
            if (state.Ss > Sfc) {
                // Calc percolation if storage exceeds field capacity storage
                Qperc = params.satdk * params.slope * (state.Ss - Sfc) / (Ssmax - Sfc);
            }

            double soil_m = state.Ss - (Qlf + Qperc);
            // calculate et
            double et_loss = calc_et(soil_m, et_params);

            // TODO: confirm this is correct and we don't need to use the (NextGen) nonlinear reservoir implementation
            state.Sgw += Qperc;
            double Qgw = params.Cgw * ( exp(params.expon * state.Sgw / params.Sgwmax) - 1 );

            // record fluxes
            fluxes.surface_runoff = surface_runoff;
            fluxes.Qlf = Qlf;
            fluxes.Qperc = Qperc;
            fluxes.Qgw = Qgw;

            // update state details
            new_state.Ss = soil_m - et_loss;
            new_state.Sgw = state.Sgw - Qgw;

            return 0;
        }

        static int mass_check(
                const tshirt_params &params,
                const tshirt_state &current_state,
                double input_flux_meters,
                const tshirt_state &next_state,
                const tshirt_fluxes &calculated_fluxes,
                double timestep_seconds)
        {
            // TODO: implement
            return 0;
        }
    };
}

//!

//!



#endif //TSHIRT_H
