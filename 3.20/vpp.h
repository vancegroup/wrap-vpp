/**
	@file vpp.h
	@brief C++ wrapper for Haption's VirtuoseAPI

	Generated by parsing version 3.20 of
	virtuoseAPI.h

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
#include <virtuoseAPI.h>
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
#ifdef VPP_VERBOSE
			std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": Constructing a new Virtuose object, device named " << _name << ", VirtContext=" << _vc << std::endl;
#endif
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

		int attachVO(float mass, float * mxmymz);
		int attachQSVO(float * Ks, float * Bs);
		int attachVOAvatar(float mass, float * mxmymz);
		int detachVO();
		int detachVOAvatar();
		int displayHardwareStatus(FILE * fh);
		int getBaseFrame(float * base);
		int getButton(int button_number, int * state);
		int getCommandType(VirtCommandType * type);
		int getDeadMan(int * dead_man);
		int getEmergencyStop(int * emergency_stop);
		int getError(int * error);
		int getErrorCode();
		int getForce(float * force);
		int getForceFactor(float * force_factor);
		int getIndexingMode(VirtIndexingType * indexing_mode);
		int getLimitTorque(float * torque);
		int getObservationFrame(float * obs);
		int getPosition(float * pos);
		int getPowerOn(int * power);
		int getSpeed(float * speed);
		int getSpeedFactor(float * speed_factor);
		int getTimeLastUpdate(int unsigned * time);
		int getTimeoutValue(float * time_value);
		int setBaseFrame(float * base);
		int setCommandType(VirtCommandType type);
		int setDebugFlags(short unsigned flag);
		int setForce(float * force);
		int setForceFactor(float force_factor);
		int setIndexingMode(VirtIndexingType indexing_mode);
		int setLimitTorque(float torque);
		int setObservationFrame(float * obs);
		int setObservationFrameSpeed(float * speed);
		int setOutputFile(char * name);
		int setPeriodicFunction(void (* fn ) (VirtContext, void *), float * period, void * arg);
		int setPosition(float * pos);
		int setPowerOn(int power);
		int setSpeed(float * speed);
		int setSpeedFactor(float speed_factor);
		int setTexture(float * position, float * intensity, int reinit);
		int setTextureForce(float * texture_force);
		int setTimeStep(float step);
		int setTimeoutValue(float time_value);
		int startLoop();
		int stopLoop();
		int waitForSynch();
		int trajRecordStart();
		int trajRecordStop();
		int trajSetSamplingTimeStep(float timeStep, int unsigned * recordTime);
		int vmStartTrajSampling(int unsigned nbSamples);
		int vmGetTrajSamples(float * samples);
		int vmSetType(VirtVmType type);
		int vmSetParameter(VirtVmParameter * param);
		int vmActivate();
		int vmDeactivate();
		int vmSetBaseFrame(float * base);
		int vmSetMaxArtiBounds(float * bounds);
		int vmSetMinArtiBounds(float * bounds);
		int getPhysicalPosition(float * pos);
		int getAvatarPosition(float * pos);
		int saturateTorque(float forceThreshold, float momentThreshold);
		int vmSetDefaultToTransparentMode();
		int vmSetDefaultToCartesianPosition();
		int vmSetBaseFrameToCurrentFrame();
		int convertRGBToGrayscale(float * rgb, float * gray);
		int vmGetBaseFrame(float * base);
		int waitPressButton(int button_number);
		int getTimeStep(float * step);
		int vmSetRobotMode(int OnOff);
		int vmSaveCurrentSpline(char * file_name);
		int vmLoadSpline(char * file_name);
		int vmDeleteSpline(char * file_name);
		int vmWaitUpperBound();
		int disableControlConnexion(int disable);
		int isInBounds(int unsigned * bounds);
		int getAlarm(int unsigned * alarm);
		int getCatchFrame(float * frame);
		int setCatchFrame(float * frame);
		int activeSpeedControl(float radius, float speedFactor);
		int deactiveSpeedControl();
		int isInShiftPosition(int * shift);
		int setFrictionForce(float fx, float fy, float fz);
		int getMouseState(int * active, int * left_click, int * right_click);
		int generateDebugFile();
		int getCenterSphere(float * pos);
		int getAxisOfRotation(float * axis);
		int getADC(int line, float * adc);
		int convertDeplToHomogeneMatrix(float * d, float * m);
		int convertHomogeneMatrixToDepl(float * d, float * m);
		int getTrackball(int * x_move, int * y_move);
		int getTrackballButton(int * active, int * left_btn, int * middle_btn, int * right_btn);
		int setAbsolutePosition(float * pos);
		int beepOnLimit(int enable);
		int enableForceFeedback(int enable);
		int getPhysicalSpeed(float * speed);
		int forceShiftButton(int forceShiftButton);
		int addForce(float * force);
		int getFailure(int unsigned * error);
		int getArticularPositionOfAdditionalAxe(float * pos);
		int setArticularPositionOfAdditionalAxe(float * pos);
		int getArticularSpeedOfAdditionalAxe(float * speed);
		int setArticularSpeedOfAdditionalAxe(float * speed);
		int setArticularForceOfAdditionalAxe(float * effort);
		/* Static Methods */
		static int APIVersion(int * major, int * minor);

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
#ifdef VPP_VERBOSE
				std::cerr << file << ":" << line << " (" << func << ")" << "Got error from Virtuose: " << getErrorMessage() << std::endl;
#endif
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

/* Wrapper Implementation Details Follow */

inline int Virtuose::APIVersion(int * major, int * minor) {
	return virtAPIVersion(major, minor);
}

inline int Virtuose::attachVO(float mass, float * mxmymz) {
	
			int ret = virtAttachVO(_vc, mass, mxmymz);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::attachQSVO(float * Ks, float * Bs) {
	
			int ret = virtAttachQSVO(_vc, Ks, Bs);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::attachVOAvatar(float mass, float * mxmymz) {
	
			int ret = virtAttachVOAvatar(_vc, mass, mxmymz);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::detachVO() {
	
			int ret = virtDetachVO(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::detachVOAvatar() {
	
			int ret = virtDetachVOAvatar(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::displayHardwareStatus(FILE * fh) {
	
			int ret = virtDisplayHardwareStatus(_vc, fh);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getBaseFrame(float * base) {
	
			int ret = virtGetBaseFrame(_vc, base);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getButton(int button_number, int * state) {
	
			int ret = virtGetButton(_vc, button_number, state);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getCommandType(VirtCommandType * type) {
	
			int ret = virtGetCommandType(_vc, type);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getDeadMan(int * dead_man) {
	
			int ret = virtGetDeadMan(_vc, dead_man);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getEmergencyStop(int * emergency_stop) {
	
			int ret = virtGetEmergencyStop(_vc, emergency_stop);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getError(int * error) {
	
			int ret = virtGetError(_vc, error);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getErrorCode() {
	
			int ret = virtGetErrorCode(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getForce(float * force) {
	
			int ret = virtGetForce(_vc, force);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getForceFactor(float * force_factor) {
	
			int ret = virtGetForceFactor(_vc, force_factor);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getIndexingMode(VirtIndexingType * indexing_mode) {
	
			int ret = virtGetIndexingMode(_vc, indexing_mode);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getLimitTorque(float * torque) {
	
			int ret = virtGetLimitTorque(_vc, torque);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getObservationFrame(float * obs) {
	
			int ret = virtGetObservationFrame(_vc, obs);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getPosition(float * pos) {
	
			int ret = virtGetPosition(_vc, pos);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getPowerOn(int * power) {
	
			int ret = virtGetPowerOn(_vc, power);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getSpeed(float * speed) {
	
			int ret = virtGetSpeed(_vc, speed);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getSpeedFactor(float * speed_factor) {
	
			int ret = virtGetSpeedFactor(_vc, speed_factor);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getTimeLastUpdate(int unsigned * time) {
	
			int ret = virtGetTimeLastUpdate(_vc, time);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getTimeoutValue(float * time_value) {
	
			int ret = virtGetTimeoutValue(_vc, time_value);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setBaseFrame(float * base) {
	
			int ret = virtSetBaseFrame(_vc, base);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setCommandType(VirtCommandType type) {
	
			int ret = virtSetCommandType(_vc, type);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setDebugFlags(short unsigned flag) {
	
			int ret = virtSetDebugFlags(_vc, flag);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setForce(float * force) {
	
			int ret = virtSetForce(_vc, force);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setForceFactor(float force_factor) {
	
			int ret = virtSetForceFactor(_vc, force_factor);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setIndexingMode(VirtIndexingType indexing_mode) {
	
			int ret = virtSetIndexingMode(_vc, indexing_mode);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setLimitTorque(float torque) {
	
			int ret = virtSetLimitTorque(_vc, torque);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setObservationFrame(float * obs) {
	
			int ret = virtSetObservationFrame(_vc, obs);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setObservationFrameSpeed(float * speed) {
	
			int ret = virtSetObservationFrameSpeed(_vc, speed);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setOutputFile(char * name) {
	
			int ret = virtSetOutputFile(_vc, name);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setPeriodicFunction(void (* fn ) (VirtContext, void *), float * period, void * arg) {
	
			int ret = virtSetPeriodicFunction(_vc, fn, period, arg);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setPosition(float * pos) {
	
			int ret = virtSetPosition(_vc, pos);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setPowerOn(int power) {
	
			int ret = virtSetPowerOn(_vc, power);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setSpeed(float * speed) {
	
			int ret = virtSetSpeed(_vc, speed);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setSpeedFactor(float speed_factor) {
	
			int ret = virtSetSpeedFactor(_vc, speed_factor);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setTexture(float * position, float * intensity, int reinit) {
	
			int ret = virtSetTexture(_vc, position, intensity, reinit);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setTextureForce(float * texture_force) {
	
			int ret = virtSetTextureForce(_vc, texture_force);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setTimeStep(float step) {
	
			int ret = virtSetTimeStep(_vc, step);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setTimeoutValue(float time_value) {
	
			int ret = virtSetTimeoutValue(_vc, time_value);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::startLoop() {
	
			int ret = virtStartLoop(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::stopLoop() {
	
			int ret = virtStopLoop(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::waitForSynch() {
	
			int ret = virtWaitForSynch(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::trajRecordStart() {
	
			int ret = virtTrajRecordStart(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::trajRecordStop() {
	
			int ret = virtTrajRecordStop(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::trajSetSamplingTimeStep(float timeStep, int unsigned * recordTime) {
	
			int ret = virtTrajSetSamplingTimeStep(_vc, timeStep, recordTime);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmStartTrajSampling(int unsigned nbSamples) {
	
			int ret = virtVmStartTrajSampling(_vc, nbSamples);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmGetTrajSamples(float * samples) {
	
			int ret = virtVmGetTrajSamples(_vc, samples);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmSetType(VirtVmType type) {
	
			int ret = virtVmSetType(_vc, type);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmSetParameter(VirtVmParameter * param) {
	
			int ret = virtVmSetParameter(_vc, param);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmActivate() {
	
			int ret = virtVmActivate(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmDeactivate() {
	
			int ret = virtVmDeactivate(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmSetBaseFrame(float * base) {
	
			int ret = virtVmSetBaseFrame(_vc, base);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmSetMaxArtiBounds(float * bounds) {
	
			int ret = virtVmSetMaxArtiBounds(_vc, bounds);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmSetMinArtiBounds(float * bounds) {
	
			int ret = virtVmSetMinArtiBounds(_vc, bounds);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getPhysicalPosition(float * pos) {
	
			int ret = virtGetPhysicalPosition(_vc, pos);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getAvatarPosition(float * pos) {
	
			int ret = virtGetAvatarPosition(_vc, pos);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::saturateTorque(float forceThreshold, float momentThreshold) {
	
			int ret = virtSaturateTorque(_vc, forceThreshold, momentThreshold);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmSetDefaultToTransparentMode() {
	
			int ret = virtVmSetDefaultToTransparentMode(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmSetDefaultToCartesianPosition() {
	
			int ret = virtVmSetDefaultToCartesianPosition(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmSetBaseFrameToCurrentFrame() {
	
			int ret = virtVmSetBaseFrameToCurrentFrame(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::convertRGBToGrayscale(float * rgb, float * gray) {
	
			int ret = virtConvertRGBToGrayscale(_vc, rgb, gray);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmGetBaseFrame(float * base) {
	
			int ret = virtVmGetBaseFrame(_vc, base);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::waitPressButton(int button_number) {
	
			int ret = virtWaitPressButton(_vc, button_number);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getTimeStep(float * step) {
	
			int ret = virtGetTimeStep(_vc, step);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmSetRobotMode(int OnOff) {
	
			int ret = virtVmSetRobotMode(_vc, OnOff);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmSaveCurrentSpline(char * file_name) {
	
			int ret = virtVmSaveCurrentSpline(_vc, file_name);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmLoadSpline(char * file_name) {
	
			int ret = virtVmLoadSpline(_vc, file_name);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmDeleteSpline(char * file_name) {
	
			int ret = virtVmDeleteSpline(_vc, file_name);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::vmWaitUpperBound() {
	
			int ret = virtVmWaitUpperBound(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::disableControlConnexion(int disable) {
	
			int ret = virtDisableControlConnexion(_vc, disable);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::isInBounds(int unsigned * bounds) {
	
			int ret = virtIsInBounds(_vc, bounds);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getAlarm(int unsigned * alarm) {
	
			int ret = virtGetAlarm(_vc, alarm);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getCatchFrame(float * frame) {
	
			int ret = virtGetCatchFrame(_vc, frame);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setCatchFrame(float * frame) {
	
			int ret = virtSetCatchFrame(_vc, frame);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::activeSpeedControl(float radius, float speedFactor) {
	
			int ret = virtActiveSpeedControl(_vc, radius, speedFactor);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::deactiveSpeedControl() {
	
			int ret = virtDeactiveSpeedControl(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::isInShiftPosition(int * shift) {
	
			int ret = virtIsInShiftPosition(_vc, shift);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setFrictionForce(float fx, float fy, float fz) {
	
			int ret = virtSetFrictionForce(_vc, fx, fy, fz);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getMouseState(int * active, int * left_click, int * right_click) {
	
			int ret = virtGetMouseState(_vc, active, left_click, right_click);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::generateDebugFile() {
	
			int ret = virtGenerateDebugFile(_vc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getCenterSphere(float * pos) {
	
			int ret = virtGetCenterSphere(_vc, pos);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getAxisOfRotation(float * axis) {
	
			int ret = virtGetAxisOfRotation(_vc, axis);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getADC(int line, float * adc) {
	
			int ret = virtGetADC(_vc, line, adc);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::convertDeplToHomogeneMatrix(float * d, float * m) {
	
			int ret = virtConvertDeplToHomogeneMatrix(_vc, d, m);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::convertHomogeneMatrixToDepl(float * d, float * m) {
	
			int ret = virtConvertHomogeneMatrixToDepl(_vc, d, m);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getTrackball(int * x_move, int * y_move) {
	
			int ret = virtGetTrackball(_vc, x_move, y_move);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getTrackballButton(int * active, int * left_btn, int * middle_btn, int * right_btn) {
	
			int ret = virtGetTrackballButton(_vc, active, left_btn, middle_btn, right_btn);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setAbsolutePosition(float * pos) {
	
			int ret = virtSetAbsolutePosition(_vc, pos);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::beepOnLimit(int enable) {
	
			int ret = virtBeepOnLimit(_vc, enable);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::enableForceFeedback(int enable) {
	
			int ret = virtEnableForceFeedback(_vc, enable);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getPhysicalSpeed(float * speed) {
	
			int ret = virtGetPhysicalSpeed(_vc, speed);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::forceShiftButton(int forceShiftButton) {
	
			int ret = virtForceShiftButton(_vc, forceShiftButton);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::addForce(float * force) {
	
			int ret = virtAddForce(_vc, force);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getFailure(int unsigned * error) {
	
			int ret = virtGetFailure(_vc, error);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getArticularPositionOfAdditionalAxe(float * pos) {
	
			int ret = virtGetArticularPositionOfAdditionalAxe(_vc, pos);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setArticularPositionOfAdditionalAxe(float * pos) {
	
			int ret = virtSetArticularPositionOfAdditionalAxe(_vc, pos);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::getArticularSpeedOfAdditionalAxe(float * speed) {
	
			int ret = virtGetArticularSpeedOfAdditionalAxe(_vc, speed);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setArticularSpeedOfAdditionalAxe(float * speed) {
	
			int ret = virtSetArticularSpeedOfAdditionalAxe(_vc, speed);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

inline int Virtuose::setArticularForceOfAdditionalAxe(float * effort) {
	
			int ret = virtSetArticularForceOfAdditionalAxe(_vc, effort);
		#ifndef VPP_DISABLE_ERROR_CHECK
			if (! ret ) {
				throw VirtuoseAPIError(std::string(__FUNCTION__) + getErrorMessage());
			}
		#endif
			return ret;
}

#endif // INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935
