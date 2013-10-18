/**
	@file
	@brief C++ wrapper for Haption's VirtuoseAPI

	Generated by parsing version UPSTREAM_VERSION_GOES_HERE of
	UPSTREAM_INCLUDE_FILENAME

	@date
	2010-2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2010-2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935
#define INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935

// Internal Includes
// none

// Library/third-party includes
#include <UPSTREAM_INCLUDE_FILENAME>

// Standard includes
#include <exception>
#include <stdexcept>
#include <string>

#ifdef VPP_VERBOSE
#	include <iostream>
#	define VPP_VERBOSE_MESSAGE(_MSG) std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": " << _MSG << std::endl
#else
#	define VPP_VERBOSE_MESSAGE(_MSG)
#endif

#ifndef VPP_DISABLE_ERROR_CHECK
#	include <sstream>
#endif

/** @brief Preprocessor definition containing an integer version of the
	VirtuoseAPI this header was generated from, with the major version
	multiplied by 1000 and added to the minor version.
*/
#define VIRTUOSEAPI_VERSION UPSTREAM_INTEGER_VERSION_GOES_HERE

/** @brief Pass arguments like 3, 60 to get a preprocessor expression
	that evaluates to true if the API used is at least that version.
*/
#define VIRTUOSEAPI_VERSION_CHECK(MAJOR, MINOR) (VIRTUOSEAPI_VERSION >= (MAJOR * 1000 + MINOR))

/** @brief Object representing a VirtuoseAPI device.

	It encapsulates and manages the lifetime of the VirtContext,
	and provides methods with names and signatures similar to the
	C API (generated from it, with minor automated modifications).

	This object is non-copyable and non-assignable: you may manage it
	with a scoped_ptr or shared_ptr and allocate it on the heap if
	those restrictions are a problem for you.
*/

class Virtuose {
	public:

#ifndef VPP_DISABLE_ERROR_CHECK
#	define VPP_CHECKED_CALL(_CALL) _checkReturnCode(_CALL, #_CALL, __FILE__, __LINE__, __FUNCTION__)
#else
#	define VPP_CHECKED_CALL(_CALL)
#endif

		struct VirtuoseAPIError : public std::runtime_error {
			VirtuoseAPIError(std::string const& what) : std::runtime_error(what) {}
		};

		/** @brief constructor

			Opens a new VirtContext upon construction.

			@param name Name of Virtuose device to connect to.

			@throws VirtuoseAPIError if opening the device failed.
		*/
		Virtuose(const std::string & name)
			: vc_(virtOpen(name.c_str()))
			, name_(name)
			, own_(true) {
			VPP_VERBOSE_MESSAGE("Constructing a new Virtuose object, device named " << name_ << ", VirtContext=" << vc_);
			if (!vc_) {
				throw VirtuoseAPIError("Failed opening Virtuose " + name_ + ": " +  getErrorMessage());
			}
		}

		/** @brief constructor from existing VirtContext

			Does not open new VirtContext. Intended for use inside a
			periodic callback function.

			@param name Name of Virtuose device to connect to.

			@throws VirtuoseAPIError if opening the device failed.
		*/
		Virtuose(VirtContext vc)
			: vc_(vc)
			, name_("unknown - from VirtContext")
			, own_(false) {
			VPP_VERBOSE_MESSAGE("Borrowing a Virtuose object with VirtContext=" << vc_);
			if (!vc_) {
				throw VirtuoseAPIError("Can't borrow a null VirtContext!");
			}
		}

		/** @brief destructor that closes the connection to the Virtuose
			device if we opened it.
		*/
		~Virtuose();

		/** @brief Conversion operator to type VirtContext.
		*/
		operator VirtContext() {
			return vc_;
		}

		/** @brief Returns the managed VirtContext.
		*/
		VirtContext getVirtContext() const {
			return vc_;
		}

		/** @brief Retrieve name used to create device context, if available.
		*/
		std::string const& getName() const {
			return name_;
		}

		/* CLASS BODY GOES HERE */

		// Use like:
		// if (virt.checkForError(ret, __FILE__, __LINE__, __FUNCTION__)) {
		//   // handle error
		// }
		bool checkForError(int returnValue, const char * file = "",
		                   int const line = -1, const char * func = "");

		bool hasError();

		/** @brief Returns the latest error message, or if none, an empty string.
		*/
		std::string getErrorOrEmpty();

		/** @brief Returns the latest error message.
		*/
		std::string getErrorMessage();

		/** @brief Converts an error code into an error message string.
		*/
		static std::string getErrorMessage(int code);

	private:
		VirtContext vc_;
		std::string const name_;
		bool const own_;

		/// @brief Copy constructor forbidden
		Virtuose(Virtuose const&);

		/// @brief Assignment operator forbidden
		Virtuose & operator=(Virtuose const&);

#ifndef VPP_DISABLE_ERROR_CHECK
		void _checkReturnCode(int returnValue, const char * call, const char * file,
		                      int const line, const char * func = "") {
			if (returnValue != 0) {
				std::ostringstream s;
				s << "VirtuoseAPI Error (in call '" << call << "' in " << func << "@" << file << ":" << line << "): " << getErrorMessage();
				throw VirtuoseAPIError(s.str());
			}
		}
#endif
};

inline 	Virtuose::~Virtuose() {
	VPP_VERBOSE_MESSAGE("In destructor for device named " << name_ << ", VirtContext=" << vc_);
	if (vc_ && own_) {
		try {
			VPP_CHECKED_CALL(virtClose(vc_));
		} catch (VirtuoseAPIError & e) {
			(void) e; // silence warning if not in verbose mode.
			VPP_VERBOSE_MESSAGE("Exception in destructor, ignoring: " << e.what());
		}
	}
}

inline bool Virtuose::checkForError(int returnValue, const char * file,
                                    int const line, const char * func) {
	if (returnValue == 0) {
		// Silence unused variable warning when VPP_VERBOSE not defined
		(void) file;
		(void) line;
		(void) func;
		return false; // no error
	} else {
		VPP_VERBOSE_MESSAGE("Got error from Virtuose (in " << func << "@" << file << ":" << line << "): "  << getErrorMessage());
		return true; // error
	}
}

inline bool Virtuose::hasError() {
	return (virtGetErrorCode(vc_) != VIRT_E_NO_ERROR);
}

inline std::string Virtuose::getErrorOrEmpty() {
	std::string ret;
	int code = virtGetErrorCode(vc_);
	if (virtGetErrorCode(vc_) != VIRT_E_NO_ERROR) {
		ret = getErrorMessage(code);
	}
	return ret;
}

/** @brief Returns the latest error message.
*/
inline std::string Virtuose::getErrorMessage() {
	return getErrorMessage(virtGetErrorCode(vc_));
}

/** @brief Converts an error code into an error message string.
*/
inline std::string Virtuose::getErrorMessage(int code) {
	return virtGetErrorMessage(code);
}

/// @brief Equality between a Virtuose object and a raw VirtContext.
inline bool operator==(Virtuose const& v, VirtContext const vc) {
	return (v.getVirtContext() == vc);
}

/// @brief Equality between a raw VirtContext and a Virtuose object.
inline bool operator==(VirtContext const vc, Virtuose const& v) {
	return (v.getVirtContext() == vc);
}

/// @brief Equality between two Virtuose objects - this should always be false!
inline bool operator==(Virtuose const& v1, Virtuose const& v2) {
	return (v1.getVirtContext() == v2.getVirtContext());
}

/// @brief Less-than comparison between Virtuose objects
inline bool operator<(Virtuose const& v1, Virtuose const& v2) {
	return (v1.getVirtContext() < v2.getVirtContext());
}

/* IMPLEMENTATION BODY GOES HERE */

#undef VPP_VERBOSE_MESSAGE
#undef VPP_CHECKED_CALL

#endif // INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935

