#pragma once

//
// test_json_model.h - Unit tests for json_model.h.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "unit_test_base.h"

#include "../platform_defs.h"

#include "../json_model.h"

PUSH_WARN_DISABLE
WARN_DISABLE(4100, "-Wunused-parameter")

namespace rda
{
    class test_json_model : public unit_test_base
    {
    protected:
        // data structure of inputs that can be re-used by each unit test
        struct unit_test_input_json_model : public unit_test_input_base
        {
        };

        // returns the name of the module that is being tested
        std::string get_test_module_name() const override
        {
            return "test_json_model";
        }

        // create and return the data structure of input to be provided to each test
        std::shared_ptr<unit_test_input_base> create_test_input(const size_t testNum, const std::string& description) override
        {
            return std::make_shared<unit_test_input_json_model>();
        }

        // setup called before each unit test is run
        void setup(size_t testNum, const std::string& description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::setup(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_json_model>(input);
        }

        // teardown called after each unit test is run
        void teardown(size_t testNum, const std::string& description, std::shared_ptr<unit_test_input_base> input) override
        {
            // call super class first
            unit_test_base::teardown(testNum, description, input);

            auto pInput = std::dynamic_pointer_cast<unit_test_input_json_model>(input);
        }

        // adds all of the tests that will be run
        void create_tests() override
        {
            add_test("primitives", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json_model>(input);

                class ChildModel : public json_model
                {
                public:
                    JSON_MEMBER_INT(ChildModel, childFieldInt, "child_int");
                };

                class ParentModel : public json_model
                {
                public:
                    JSON_MEMBER_BOOL(   ParentModel,             parentFieldBool,   "parent_bool");
                    JSON_MEMBER_INT(    ParentModel,             parentFieldInt,    "parent_int");
                    JSON_MEMBER_FLOAT(  ParentModel,             parentFieldFloat,  "parent_float");
                    JSON_MEMBER_STRING( ParentModel,             parentFieldString, "parent_string");
                };

                ParentModel parentModel;

                parentModel.parentFieldBool(false);
                ASSERT_TRUE(parentModel.parentFieldBool() == false);

                parentModel.parentFieldInt(-8675309);
                ASSERT_TRUE(parentModel.parentFieldInt() == -8675309);

                parentModel.parentFieldFloat(2.787878);
                ASSERT_FLOAT_EQUALS(parentModel.parentFieldFloat(), 2.787878);

                parentModel.parentFieldString("hello world");
                ASSERT_TRUE(parentModel.parentFieldString() == std::string("hello world"));

                //std::cout << std::endl << s.toPrettyJSON() << std::endl;
                });

            add_test("sub-object children", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json_model>(input);

                class ChildModel : public json_model
                {
                public:
                    JSON_MEMBER_INT(ChildModel, childFieldInt, "child_int");
                };

                class ParentModel : public json_model
                {
                public:
                    JSON_MEMBER_OBJECT(ParentModel, ChildModel, parentFieldChild, "parent_child");
                };

                ParentModel parentModel;

                ChildModel childModel;
                childModel.childFieldInt(314);
                parentModel.parentFieldChild(childModel);
                ChildModel retrievedChildModel = parentModel.parentFieldChild();
                ASSERT_TRUE(childModel.childFieldInt() == 314);
                ASSERT_TRUE(retrievedChildModel.childFieldInt() == 314);

                //std::cout << std::endl << s.toPrettyJSON() << std::endl;
                });

            add_test("arrays of primitives", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json_model>(input);

                class ChildModel : public json_model
                {
                public:
                    JSON_MEMBER_INT(ChildModel, childFieldInt, "child_int");
                };

                class ParentModel : public json_model
                {
                public:
                    JSON_MEMBER_ARRAY_BOOL(   ParentModel,             parentFieldBoolArray,   "bool_vector_name");
                    JSON_MEMBER_ARRAY_INT(    ParentModel,             parentFieldIntArray,    "int_vector_name");
                    JSON_MEMBER_ARRAY_FLOAT(  ParentModel,             parentFieldFloatArray,  "float_vector_name");
                    JSON_MEMBER_ARRAY_STRING( ParentModel,             parentFieldStringArray, "str_vector_name");
                };

                ParentModel parentModel;

                std::vector<bool> boolArray1 = { true, true, false, true, false, true };
                parentModel.parentFieldBoolArray(boolArray1);
                auto boolArray2 = parentModel.parentFieldBoolArray();
                ASSERT_TRUE(boolArray1 == boolArray2);

                std::vector<int64_t> intArray1 = { 2, 3, 1, -6, 8, 0, 0, -88 };
                parentModel.parentFieldIntArray(intArray1);
                auto intArray2 = parentModel.parentFieldIntArray();
                ASSERT_TRUE(intArray1 == intArray2);

                std::vector<double> floatArray1 = { 0.0, 1.0, 3.5, 3.1415, -1.0, -5.789 };
                parentModel.parentFieldFloatArray(floatArray1);
                auto floatArray2 = parentModel.parentFieldFloatArray();
                ASSERT_TRUE(floatArray1.size() == floatArray2.size());
                for (std::size_t i = 0; i < floatArray1.size(); ++i)
                    ASSERT_FLOAT_EQUALS(floatArray1[i], floatArray2[i]);

                std::vector<std::string> stringArray1 = { "hello", "world", "", "abc", "test string" };
                parentModel.parentFieldStringArray(stringArray1);
                auto stringArray2 = parentModel.parentFieldStringArray();
                ASSERT_TRUE(stringArray1 == stringArray2);

                //std::cout << std::endl << s.toPrettyJSON() << std::endl;
                });
            
            add_test("array of child objects", [](std::shared_ptr<unit_test_input_base> input) {
                auto pInput = std::dynamic_pointer_cast<unit_test_input_json_model>(input);

                class ChildModel : public json_model
                {
                public:
                    JSON_MEMBER_INT(ChildModel, childFieldInt, "child_int");
                };

                class ParentModel : public json_model
                {
                public:
                    JSON_MEMBER_ARRAY_OBJECT(ParentModel, ChildModel, parentFieldObjectArray, "str_vector_name");
                };

                ParentModel parentModel;

                std::vector<ChildModel> childArray1;

                ChildModel child1;
                child1.childFieldInt(101);
                childArray1.push_back(child1);

                ChildModel child2;
                child2.childFieldInt(102);
                childArray1.push_back(child2);

                ChildModel child3;
                child3.childFieldInt(103);
                childArray1.push_back(child3);

                parentModel.parentFieldObjectArray(childArray1);
                auto childArray2 = parentModel.parentFieldObjectArray();

                ASSERT_TRUE(childArray1.size() == childArray2.size());
                for (std::size_t i = 0; i < childArray1.size(); ++i)
                {
                    ASSERT_TRUE(childArray1[i].childFieldInt() == childArray2[i].childFieldInt());
                    ASSERT_TRUE(childArray2[i].childFieldInt() == static_cast<int64_t>(101 + i));
                }

                //std::cout << std::endl << s.toPrettyJSON() << std::endl;
                });
        }
    }; // class test_serializable

} // namespace rda

POP_WARN_DISABLE
