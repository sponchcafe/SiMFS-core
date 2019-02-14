#include "shape/eField.hpp"

namespace sim{
    namespace focus{

        //------------------------------------------------------------------//
        EField::EField(){ } 

        //------------------------------------------------------------------//
        void EField::set_lambda(double l){ 
            k = 2*CONST_PI/l;
        }

        //------------------------------------------------------------------//
        void EField::set_pol(bool px, bool py){ 
            pol_x = px;
            pol_y = py;
        }

        //------------------------------------------------------------------//
        void EField::set_amplitude(double e0){ 
            e_zero = e0;
        }


        //------------------------------------------------------------------//
        void EField::set_theta(double min, double max, size_t n){ 
            theta_min_deg = min;
            theta_max_deg = max;
            theta_min = min/360*2*CONST_PI;
            theta_max = max/360*2*CONST_PI;
            theta_n = n;
        }

        //------------------------------------------------------------------//
        void EField::set_phi(double min, double max, size_t n){ 
            phi_min_deg = min;
            phi_max_deg = max;
            phi_min = min/360*2*CONST_PI;
            phi_max = max/360*2*CONST_PI;
            phi_n = n;
        }

        //------------------------------------------------------------------//
        void EField::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_lambda(params.at("wavelength"));
            json pol = params.at("polarization");
            set_pol(pol.at("x"), pol.at("y"));
            json theta = params.at("theta");
            set_theta(theta.at("min"), theta.at("max"), theta.at("n"));
            json phi = params.at("phi");
            set_phi(phi.at("min"), phi.at("max"), phi.at("n"));

        }

        //------------------------------------------------------------------//
        json EField::get_json(){

            json j;

            j["wavelength"] = 2*CONST_PI/k;
            j["polarization"] = json{
                {"x", pol_x},
                {"y", pol_y}
            };
            j["theta"] = json{
                {"min", theta_min_deg},
                {"max", theta_max_deg},
                {"n", theta_n}
            };
            j["phi"] = json{
                {"min", phi_min_deg},
                {"max", phi_max_deg},
                {"n", phi_n}
            };

            return j;
        }

        //-------------------------------------------------------------------//
        double EField::evaluate(double x, double y, double z) const {

            EFieldComponents field = evaluate_field(x, y, z);

            double res = 0.0;

            res += std::norm(field.x);
            res += std::norm(field.y);
            res += std::norm(field.z);

            return res;

        }

        //------------------------------------------------------------------//
        double EField::get_flux_density_prefactor() const {
            return 1.0;
        };

        //------------------------------------------------------------------//
        double EField::get_efficiency_prefactor() const {
            return evaluate(0,0,0);
        }

        //------------------------------------------------------------------//
        void EField::init(){

            d_theta = (theta_max-theta_min) / theta_n;
            for(size_t i = 0; i<theta_n; i++){
                sin_theta.push_back(sin(i*d_theta));
                cos_theta.push_back(cos(i*d_theta));
            }

            d_phi = (phi_max-phi_min) / phi_n;
            for(size_t i = 0; i<phi_n; i++){
                sin_phi.push_back(sin(i*d_phi));
                cos_phi.push_back(cos(i*d_phi));
            }

        }

        //------------------------------------------------------------------//
        EField::EFieldComponents EField::evaluate_angle(double x, double y, double z, size_t i_phi, size_t i_theta) const {

            double sp = sin_phi[i_phi];
            double cp = cos_phi[i_phi];
            double st = sin_theta[i_theta];
            double ct = cos_theta[i_theta];

            EFieldComponents field{};

            double a1 = -sp * pol_x + cp * pol_y;
            double a2 =  cp * pol_x + sp * pol_y;
            double a3 = k * (-st * cp * x - st * sp * y + ct * z);
            std::complex<double> a4 = st * sqrt(ct) * e_zero * ( cos(a3) + imag_unit * sin(a3));

            field.x = (-sp * a1 + ct * cp * a2) * a4;
            field.y = ( cp * a1 + ct * sp * a2) * a4;
            field.z = (           st      * a2) * a4;
            
            return field;

        }

        //------------------------------------------------------------------//
        EField::EFieldComponents EField::evaluate_field(double x, double y, double z) const {

            EFieldComponents field{};

            for (size_t p=0; p<phi_n; p++){
                for (size_t t=0; t<theta_n; t++){

                    EFieldComponents _field = evaluate_angle(x, y, z, p, t);
                    field.x += _field.x * d_phi * d_theta;
                    field.y += _field.y * d_phi * d_theta;
                    field.z += _field.z * d_phi * d_theta;
                    
                }
            }

            return field;
            
        }

    }
}
