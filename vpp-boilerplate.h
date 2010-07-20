/**
	@file vpp.h
	@brief C++ wrapper for Haption's VirtuoseAPI

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
#include <VirtuoseAPI.h>
//}

// Standard includes
#include <exception>
#include <stdexcept>
#include <string>

#ifdef VPP_VERBOSE
#include <iostream>
#endif

class Virtuose {
	public:
		//using _VAPI::VirtContext;

		/** @brief constructor

			Opens a new VirtContext upon construction.

			@param name Name of Virtuose device to connect to.

			@throws std::runtime_error if opening the device failed.
		*/
		Virtuose(const std::string & name) :
			_name(name),
			_vc(virtOpen(_name.c_str())),
			_weOpened(true)
		{
#ifdef VPP_VERBOSE
			std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": Constructing a new Virtuose object, device named " << _name << ", VirtContext=" << _vc << std::endl;
#endif
			if (_vc == NULL) {
				_weOpened = false;
				throw std::runtime_error("Failed opening Virtuose " + _name + getErrorMessage());
			}
		}

		/// @brief Constructor from an existing Virtuose
		Virtuose(VirtContext vc) :
			_name("n/a"),
			_vc(vc),
			_weOpened(false)
		{
#ifdef VPP_VERBOSE
			std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": Constructing a Virtuose object from existing VirtContext " << _vc << std::endl;
#endif		
		}

		/// @brief Copy constructor
		Virtuose(Virtuose const& other) :
			_name(other._name),
			_vc(other._vc),
			_weOpened(false)
		{
#ifdef VPP_VERBOSE
			std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": Copy-constructing a Virtuose object from existing Virtuose object, named " << _name << ", VirtContext=" << _vc << std::endl;
#endif
		}

		/// @brief Assignment operator
		Virtuose & operator=(Virtuose const& other) {
			if (&other == this) {
				// Self assignment - no-op
#ifdef VPP_VERBOSE
				std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": Self-assignment is a no-op" << std::endl;
#endif
			} else {
				// Actual assignment
				int ret = 0;
				if (_weOpened) {
					// Close our existing one first.
#ifdef VPP_VERBOSE
					std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": In assignment operator, closing existing Virtuose device named " << _name << ", VirtContext=" << _vc << std::endl;
#endif
					ret = virtClose(_vc);
				}
				if (!checkForError(ret, __FILE__, __LINE__, __FUNCTION__)) {
					_name = other._name;
					_vc = other._vc;
					_weOpened = false;
	#ifdef VPP_VERBOSE
					std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": Assignment operator has set ourselves to a device named " << _name << ", VirtContext=" << _vc << std::endl;
	#endif
				}
			}
			return *this;
		}

		/** @brief destructor that closes the connection to the Virtuose
			device.
		*/
		~Virtuose() {
			if (_weOpened) {
#ifdef VPP_VERBOSE
			std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": In destructor for device named " << _name << ", VirtContext=" << _vc << ", closing because _weOpened flag is set" << std::endl;
#endif
				int ret = virtClose(_vc);
				checkForError(ret, __FILE__, __LINE__, __FUNCTION__);
			} else {
#ifdef VPP_VERBOSE
				std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": In destructor for device named " << _name << ", VirtContext=" << _vc << ", NOT closing because _weOpened flag is not set" << std::endl;
#endif
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
				std::cerr << file << ":" << line << " (" << func << ")" << "Got error from Virtuose: " << getErrorMessage() << std::endl;
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

