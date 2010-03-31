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
#ifndef _VPP_HXX
#define _VPP_HXX

// Standard includes
#include <exception>
#include <stdexcept>
#include <string>

// Library/third-party includes
namespace _VAPI {
#include <VirtuoseAPI.h>
}
// Internal Includes
// none

class Virtuose {
	public:
		using _VAPI::VirtContext;

		/** @brief constructor

			Opens a new VirtContext upon construction.

			@param name Name of Virtuose device to connect to.

			@throws std::runtime_error if opening the device failed.
		*/
		Virtuose(const std::string & name) :
			m_name(name),
			m_vc(_VAPI::virtOpen(m_name.c_str())
		{
			if (!m_vc) {
				throw std::runtime_error("Failed opening Virtuose " + m_name);
			}
		}

		/** @brief destructor that closes the connection to the Virtuose
			device.
		*/
		~Virtuose() {
			_VAPI::virtClose(m_vc);
		}

		VirtContext() {
			return m_vc;
		}

		/* CLASS BODY GOES HERE */

		static std::string getErrorMessage(int code) {
			return std::string(_VAPI::virtGetErrorMessage(code));
		}

	protected:
		std::string m_name;
		VirtContext m_vc;
};

/* IMPLEMENTATION BODY GOES HERE */

#endif // _VPP_HXX
