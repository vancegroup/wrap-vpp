/**
	@file vpp.h
	@brief C++ wrapper for Haption's VirtuoseAPI

	Generated by parsing version UPSTREAM_VERSION_GOES_HERE of
	UPSTREAM_INCLUDE_FILENAME

	@date
	2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
#pragma once
#ifndef INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935
#define INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935

// Internal Includes
// none

// Library/third-party includes
/// @todo use namespace
//namespace _VAPI {
#include <UPSTREAM_INCLUDE_FILENAME>
//}

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

class Virtuose {
	public:
		//using _VAPI::VirtContext;
		
		struct VirtuoseAPIError : public std::runtime_error {
			VirtuoseAPIError(std::string const& what) : std::runtime_error(what) {}
		};

		/** @brief constructor

			Opens a new VirtContext upon construction.

			@param name Name of Virtuose device to connect to.

			@throws VirtuoseAPIError if opening the device failed.
		*/
		Virtuose(const std::string & name) :
			_name(name),
			_vc(virtOpen(_name.c_str())),
			_weOpened(true)
		{
			VPP_VERBOSE_MESSAGE("Constructing a new Virtuose object, device named " << _name << ", VirtContext=" << _vc);
			if (_vc == NULL) {
				_weOpened = false;
				throw VirtuoseAPIError("Failed opening Virtuose " + _name + getErrorMessage());
			}
		}

		/// @brief Constructor from an existing Virtuose
		Virtuose(VirtContext vc) :
			_name("n/a"),
			_vc(vc),
			_weOpened(false)
		{
			VPP_VERBOSE_MESSAGE("Constructing a Virtuose object from existing VirtContext " << _vc);
		}

		/// @brief Copy constructor
		Virtuose(Virtuose const& other) :
			_name(other._name),
			_vc(other._vc),
			_weOpened(false)
		{
			VPP_VERBOSE_MESSAGE("Copy-constructing a Virtuose object from existing Virtuose object, named " << _name << ", VirtContext=" << _vc);
		}

		/// @brief Assignment operator
		Virtuose & operator=(Virtuose const& other) {
			if (&other == this) {
				// Self assignment - no-op
				VPP_VERBOSE_MESSAGE("Self-assignment is a no-op");
			} else {
				// Actual assignment
				int ret = 0;
				if (_weOpened) {
					// Close our existing one first.
					VPP_VERBOSE_MESSAGE("In assignment operator, closing existing Virtuose device named " << _name << ", VirtContext=" << _vc);
					ret = virtClose(_vc);
				}
				if (!checkForError(ret, __FILE__, __LINE__, __FUNCTION__)) {
					_name = other._name;
					_vc = other._vc;
					_weOpened = false;
					VPP_VERBOSE_MESSAGE("Assignment operator has set ourselves to a device named " << _name << ", VirtContext=" << _vc);
				}
			}
			return *this;
		}

		/** @brief destructor that closes the connection to the Virtuose
			device.
		*/
		~Virtuose() {
			if (_weOpened) {
				VPP_VERBOSE_MESSAGE("In destructor for device named " << _name << ", VirtContext=" << _vc << ", closing because _weOpened flag is set");
				int ret = virtClose(_vc);
				checkForError(ret, __FILE__, __LINE__, __FUNCTION__);
			} else {
				VPP_VERBOSE_MESSAGE("In destructor for device named " << _name << ", VirtContext=" << _vc << ", NOT closing because _weOpened flag is not set");
			}
		}

		operator VirtContext() {
			return _vc;
		}

		/* CLASS BODY GOES HERE */

		// Use like:
		// if (virt.checkForError(ret, __FILE__, __LINE__, __FUNCTION__)) {
		//   // handle error
		// }
		bool checkForError(int returnValue, std::string const&
file = "", int const line = -1, std::string const&
func
= "") {
			if (returnValue == 0) {
				return false; // no error
			} else {
				VPP_VERBOSE_MESSAGE("Got error from Virtuose: " << getErrorMessage());
				return true; // error
			}
		}

		std::string getErrorMessage() {
			return std::string(virtGetErrorMessage(virtGetErrorCode(_vc)));
		}

		static std::string getErrorMessage(int code) {
			return std::string(virtGetErrorMessage(code));
		}

	protected:
		std::string _name;
		VirtContext _vc;
		bool _weOpened;
};

/* IMPLEMENTATION BODY GOES HERE */

#endif // INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935

