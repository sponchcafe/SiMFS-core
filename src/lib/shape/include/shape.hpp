#pragma once

#include <string>
#include <stdexcept>
#include "json/json.hpp"

#include "shape/XYZGauss.hpp"
#include "shape/XYGaussZExp.hpp"
#include "shape/gaussBeam.hpp"
#include "shape/genericGrid.hpp"
#include "shape/eField.hpp"


namespace sim{
    namespace focus{

        using json = nlohmann::json;

        //-Enums-----------------------------------------------------//
        enum class FocusMode 
        {
            EXCITATION, 
            DETECTION, 
            GENERIC
        };

        const std::map<std::string, FocusMode> modes{
            {"excitation", FocusMode::EXCITATION},
            {"detection", FocusMode::DETECTION},
            {"generic", FocusMode::GENERIC}
        };

        enum class ShapeType 
        {
            XYZ_GAUSS, 
            XY_GAUSS_Z_EXP, 
            GAUSS_BEAM, 
            E_FIELD, 
            GENERIC_GRID,
            NONE = -1
        };

        NLOHMANN_JSON_SERIALIZE_ENUM( ShapeType, {
            {ShapeType::XYZ_GAUSS, "3dGauss"},
            {ShapeType::XY_GAUSS_Z_EXP, "XYGaussZExp"},
            {ShapeType::GAUSS_BEAM, "gaussBeam"},
            {ShapeType::E_FIELD, "eField"},
            {ShapeType::GENERIC_GRID, "grid"},
            {ShapeType::NONE, "__none__"}
        })


        //-----------------------------------------------------------//
        inline static std::unique_ptr<FocusShape> focus_shape_factory(
                ShapeType type,
                json params=json::object()){

            std::unique_ptr<FocusShape> shape_ptr;
            switch (type) {
                case ShapeType::XYZ_GAUSS:
                    shape_ptr = std::make_unique<XYZGauss>(); 
                    break;
                case ShapeType::XY_GAUSS_Z_EXP:
                    shape_ptr = std::make_unique<XYGaussZExp>(); 
                    break;
                case ShapeType::GAUSS_BEAM:
                    shape_ptr = std::make_unique<GaussBeam>(); 
                    break;
                case ShapeType::E_FIELD:
                    shape_ptr = std::make_unique<EField>(); 
                    break;
                case ShapeType::GENERIC_GRID:
                    shape_ptr = std::make_unique<GenericGrid>(); 
                    break;
                default:
                    //std::cerr << "No such focus shape: " << type << '\n';
                    shape_ptr = std::make_unique<XYZGauss>();
                    break;

            }

            shape_ptr->set_json(params);
            return shape_ptr;

        }


    }
}
