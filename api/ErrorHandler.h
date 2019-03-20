#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <string>

#include "visibility.h"
#include "SAM_api.h"

extern "C" {


struct error {
    std::string message;
};

const char* error_message(SAM_error error);

void error_destruct(SAM_error error);

}

/// Returns true if fn executed without throwing an error, false otherwise.
/// If calling fn threw an error, capture it in *out_error.
template<typename Fn>
static bool translateExceptions(SAM_error* out_error, Fn&& fn)
{
    try {
        fn();
    } catch (const std::exception& e) {
        *out_error = new error{e.what()};
        return false;
    } catch (const std::runtime_error& e){
        *out_error = new error{e.what()};
        return false;
    }
    catch (...) {
        *out_error = new error{"Unknown internal error"};
        return false;
    }
    return true;
}

void make_access_error(std::string obj_name, std::string var);