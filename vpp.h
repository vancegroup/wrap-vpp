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
			m_name(name),
			m_vc(virtOpen(m_name.c_str())),
			_weOpened(true)
		{
#ifdef VPP_VERBOSE
			std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": Constructing a new Virtuose object, device named " << name << ", VirtContext=" << m_vc << std::endl;
#endif
			if (!m_vc) {
				throw std::runtime_error("Failed opening Virtuose " + m_name);
			}
		}

		/// @brief Constructor from an existing Virtuose
		Virtuose(VirtContext vc) :
			m_name("n/a"),
			m_vc(vc),
			_weOpened(false)
		{
#ifdef VPP_VERBOSE
			std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": Constructing a Virtuose object from existing VirtContext " << m_vc << std::endl;
#endif		
		}

		/// @brief Copy constructor
		Virtuose(Virtuose const& other) :
			m_name(other.m_name),
			m_vc(other.m_vc),
			_weOpened(false)
		{
#ifdef VPP_VERBOSE
			std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": Copy-constructing a Virtuose object from existing Virtuose object, named " << name << ", VirtContext=" << m_vc << std::endl;
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
				if (_weOpened) {
					// Close our existing one first.
#ifdef VPP_VERBOSE
					std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": In assignment operator, closing existing Virtuose device named " << name << ", VirtContext=" << m_vc << std::endl;
#endif
					virtClose(m_vc);
				}
				m_name = other.m_name;
				m_vc = other.m_vc;
				_weOpened = false;
#ifdef VPP_VERBOSE
				std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": Assignment operator has set ourselves to a device named " << name << ", VirtContext=" << m_vc << std::endl;
#endif
			}
			return *this;
		}

		/** @brief destructor that closes the connection to the Virtuose
			device.
		*/
		~Virtuose() {
			if (_weOpened) {
#ifdef VPP_VERBOSE
			std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": In destructor for device named " << name << ", VirtContext=" << m_vc << ", closing because _weOpened flag is set" << std::endl;
#endif
				virtClose(m_vc);
			} else {
#ifdef VPP_VERBOSE
				std::cout << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ")" << ": In destructor for device named " << name << ", VirtContext=" << m_vc << ", NOT closing because _weOpened flag is not set" << std::endl;
#endif
			}
		}

		operator VirtContext() {
			return m_vc;
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
		/* Static Methods */
		static int APIVersion(int * major, int * minor);
		
		std::string getErrorMessage() {
			return std::string(virtGetErrorMessage(virtGetErrorCode(m_vc)));
		}
		
		static std::string getErrorMessage(int code) {
			return std::string(virtGetErrorMessage(code));
		}

	protected:
		std::string m_name;
		VirtContext m_vc;
		bool _weOpened;
};

/* Wrapper Implementation Details Follow */

inline int Virtuose::APIVersion(int * major, int * minor) {
	return virtAPIVersion(major, minor);
}

inline int Virtuose::attachVO(float mass, float * mxmymz) {
	return virtAttachVO(m_vc, mass, mxmymz);
}

inline int Virtuose::attachQSVO(float * Ks, float * Bs) {
	return virtAttachQSVO(m_vc, Ks, Bs);
}

inline int Virtuose::attachVOAvatar(float mass, float * mxmymz) {
	return virtAttachVOAvatar(m_vc, mass, mxmymz);
}

inline int Virtuose::detachVO() {
	return virtDetachVO(m_vc);
}

inline int Virtuose::detachVOAvatar() {
	return virtDetachVOAvatar(m_vc);
}

inline int Virtuose::displayHardwareStatus(FILE * fh) {
	return virtDisplayHardwareStatus(m_vc, fh);
}

inline int Virtuose::getBaseFrame(float * base) {
	return virtGetBaseFrame(m_vc, base);
}

inline int Virtuose::getButton(int button_number, int * state) {
	return virtGetButton(m_vc, button_number, state);
}

inline int Virtuose::getCommandType(VirtCommandType * type) {
	return virtGetCommandType(m_vc, type);
}

inline int Virtuose::getDeadMan(int * dead_man) {
	return virtGetDeadMan(m_vc, dead_man);
}

inline int Virtuose::getEmergencyStop(int * emergency_stop) {
	return virtGetEmergencyStop(m_vc, emergency_stop);
}

inline int Virtuose::getError(int * error) {
	return virtGetError(m_vc, error);
}

inline int Virtuose::getErrorCode() {
	return virtGetErrorCode(m_vc);
}

inline int Virtuose::getForce(float * force) {
	return virtGetForce(m_vc, force);
}

inline int Virtuose::getForceFactor(float * force_factor) {
	return virtGetForceFactor(m_vc, force_factor);
}

inline int Virtuose::getIndexingMode(VirtIndexingType * indexing_mode) {
	return virtGetIndexingMode(m_vc, indexing_mode);
}

inline int Virtuose::getLimitTorque(float * torque) {
	return virtGetLimitTorque(m_vc, torque);
}

inline int Virtuose::getObservationFrame(float * obs) {
	return virtGetObservationFrame(m_vc, obs);
}

inline int Virtuose::getPosition(float * pos) {
	return virtGetPosition(m_vc, pos);
}

inline int Virtuose::getPowerOn(int * power) {
	return virtGetPowerOn(m_vc, power);
}

inline int Virtuose::getSpeed(float * speed) {
	return virtGetSpeed(m_vc, speed);
}

inline int Virtuose::getSpeedFactor(float * speed_factor) {
	return virtGetSpeedFactor(m_vc, speed_factor);
}

inline int Virtuose::getTimeLastUpdate(int unsigned * time) {
	return virtGetTimeLastUpdate(m_vc, time);
}

inline int Virtuose::getTimeoutValue(float * time_value) {
	return virtGetTimeoutValue(m_vc, time_value);
}

inline int Virtuose::setBaseFrame(float * base) {
	return virtSetBaseFrame(m_vc, base);
}

inline int Virtuose::setCommandType(VirtCommandType type) {
	return virtSetCommandType(m_vc, type);
}

inline int Virtuose::setDebugFlags(short unsigned flag) {
	return virtSetDebugFlags(m_vc, flag);
}

inline int Virtuose::setForce(float * force) {
	return virtSetForce(m_vc, force);
}

inline int Virtuose::setForceFactor(float force_factor) {
	return virtSetForceFactor(m_vc, force_factor);
}

inline int Virtuose::setIndexingMode(VirtIndexingType indexing_mode) {
	return virtSetIndexingMode(m_vc, indexing_mode);
}

inline int Virtuose::setLimitTorque(float torque) {
	return virtSetLimitTorque(m_vc, torque);
}

inline int Virtuose::setObservationFrame(float * obs) {
	return virtSetObservationFrame(m_vc, obs);
}

inline int Virtuose::setObservationFrameSpeed(float * speed) {
	return virtSetObservationFrameSpeed(m_vc, speed);
}

inline int Virtuose::setOutputFile(char * name) {
	return virtSetOutputFile(m_vc, name);
}

inline int Virtuose::setPeriodicFunction(void (* fn ) (VirtContext, void *), float * period, void * arg) {
	return virtSetPeriodicFunction(m_vc, fn, period, arg);
}

inline int Virtuose::setPosition(float * pos) {
	return virtSetPosition(m_vc, pos);
}

inline int Virtuose::setPowerOn(int power) {
	return virtSetPowerOn(m_vc, power);
}

inline int Virtuose::setSpeed(float * speed) {
	return virtSetSpeed(m_vc, speed);
}

inline int Virtuose::setSpeedFactor(float speed_factor) {
	return virtSetSpeedFactor(m_vc, speed_factor);
}

inline int Virtuose::setTexture(float * position, float * intensity, int reinit) {
	return virtSetTexture(m_vc, position, intensity, reinit);
}

inline int Virtuose::setTextureForce(float * texture_force) {
	return virtSetTextureForce(m_vc, texture_force);
}

inline int Virtuose::setTimeStep(float step) {
	return virtSetTimeStep(m_vc, step);
}

inline int Virtuose::setTimeoutValue(float time_value) {
	return virtSetTimeoutValue(m_vc, time_value);
}

inline int Virtuose::startLoop() {
	return virtStartLoop(m_vc);
}

inline int Virtuose::stopLoop() {
	return virtStopLoop(m_vc);
}

inline int Virtuose::waitForSynch() {
	return virtWaitForSynch(m_vc);
}

inline int Virtuose::trajRecordStart() {
	return virtTrajRecordStart(m_vc);
}

inline int Virtuose::trajRecordStop() {
	return virtTrajRecordStop(m_vc);
}

inline int Virtuose::trajSetSamplingTimeStep(float timeStep, int unsigned * recordTime) {
	return virtTrajSetSamplingTimeStep(m_vc, timeStep, recordTime);
}

inline int Virtuose::vmStartTrajSampling(int unsigned nbSamples) {
	return virtVmStartTrajSampling(m_vc, nbSamples);
}

inline int Virtuose::vmGetTrajSamples(float * samples) {
	return virtVmGetTrajSamples(m_vc, samples);
}

inline int Virtuose::vmSetType(VirtVmType type) {
	return virtVmSetType(m_vc, type);
}

inline int Virtuose::vmSetParameter(VirtVmParameter * param) {
	return virtVmSetParameter(m_vc, param);
}

inline int Virtuose::vmActivate() {
	return virtVmActivate(m_vc);
}

inline int Virtuose::vmDeactivate() {
	return virtVmDeactivate(m_vc);
}

inline int Virtuose::vmSetBaseFrame(float * base) {
	return virtVmSetBaseFrame(m_vc, base);
}

inline int Virtuose::vmSetMaxArtiBounds(float * bounds) {
	return virtVmSetMaxArtiBounds(m_vc, bounds);
}

inline int Virtuose::vmSetMinArtiBounds(float * bounds) {
	return virtVmSetMinArtiBounds(m_vc, bounds);
}

inline int Virtuose::getPhysicalPosition(float * pos) {
	return virtGetPhysicalPosition(m_vc, pos);
}

inline int Virtuose::getAvatarPosition(float * pos) {
	return virtGetAvatarPosition(m_vc, pos);
}

inline int Virtuose::saturateTorque(float forceThreshold, float momentThreshold) {
	return virtSaturateTorque(m_vc, forceThreshold, momentThreshold);
}

inline int Virtuose::vmSetDefaultToTransparentMode() {
	return virtVmSetDefaultToTransparentMode(m_vc);
}

inline int Virtuose::vmSetDefaultToCartesianPosition() {
	return virtVmSetDefaultToCartesianPosition(m_vc);
}

inline int Virtuose::vmSetBaseFrameToCurrentFrame() {
	return virtVmSetBaseFrameToCurrentFrame(m_vc);
}

inline int Virtuose::convertRGBToGrayscale(float * rgb, float * gray) {
	return virtConvertRGBToGrayscale(m_vc, rgb, gray);
}

inline int Virtuose::vmGetBaseFrame(float * base) {
	return virtVmGetBaseFrame(m_vc, base);
}

inline int Virtuose::waitPressButton(int button_number) {
	return virtWaitPressButton(m_vc, button_number);
}

inline int Virtuose::getTimeStep(float * step) {
	return virtGetTimeStep(m_vc, step);
}

inline int Virtuose::vmSetRobotMode(int OnOff) {
	return virtVmSetRobotMode(m_vc, OnOff);
}

inline int Virtuose::vmSaveCurrentSpline(char * file_name) {
	return virtVmSaveCurrentSpline(m_vc, file_name);
}

inline int Virtuose::vmLoadSpline(char * file_name) {
	return virtVmLoadSpline(m_vc, file_name);
}

inline int Virtuose::vmDeleteSpline(char * file_name) {
	return virtVmDeleteSpline(m_vc, file_name);
}

inline int Virtuose::vmWaitUpperBound() {
	return virtVmWaitUpperBound(m_vc);
}

inline int Virtuose::disableControlConnexion(int disable) {
	return virtDisableControlConnexion(m_vc, disable);
}

inline int Virtuose::isInBounds(int unsigned * bounds) {
	return virtIsInBounds(m_vc, bounds);
}

inline int Virtuose::getAlarm(int unsigned * alarm) {
	return virtGetAlarm(m_vc, alarm);
}

inline int Virtuose::getCatchFrame(float * frame) {
	return virtGetCatchFrame(m_vc, frame);
}

inline int Virtuose::setCatchFrame(float * frame) {
	return virtSetCatchFrame(m_vc, frame);
}

inline int Virtuose::activeSpeedControl(float radius, float speedFactor) {
	return virtActiveSpeedControl(m_vc, radius, speedFactor);
}

inline int Virtuose::deactiveSpeedControl() {
	return virtDeactiveSpeedControl(m_vc);
}

inline int Virtuose::isInShiftPosition(int * shift) {
	return virtIsInShiftPosition(m_vc, shift);
}

inline int Virtuose::setFrictionForce(float fx, float fy, float fz) {
	return virtSetFrictionForce(m_vc, fx, fy, fz);
}

inline int Virtuose::getMouseState(int * active, int * left_click, int * right_click) {
	return virtGetMouseState(m_vc, active, left_click, right_click);
}

inline int Virtuose::generateDebugFile() {
	return virtGenerateDebugFile(m_vc);
}

inline int Virtuose::getCenterSphere(float * pos) {
	return virtGetCenterSphere(m_vc, pos);
}

inline int Virtuose::getAxisOfRotation(float * axis) {
	return virtGetAxisOfRotation(m_vc, axis);
}

inline int Virtuose::getADC(int line, float * adc) {
	return virtGetADC(m_vc, line, adc);
}

inline int Virtuose::convertDeplToHomogeneMatrix(float * d, float * m) {
	return virtConvertDeplToHomogeneMatrix(m_vc, d, m);
}

inline int Virtuose::convertHomogeneMatrixToDepl(float * d, float * m) {
	return virtConvertHomogeneMatrixToDepl(m_vc, d, m);
}

inline int Virtuose::getTrackball(int * x_move, int * y_move) {
	return virtGetTrackball(m_vc, x_move, y_move);
}

inline int Virtuose::getTrackballButton(int * active, int * left_btn, int * middle_btn, int * right_btn) {
	return virtGetTrackballButton(m_vc, active, left_btn, middle_btn, right_btn);
}

inline int Virtuose::setAbsolutePosition(float * pos) {
	return virtSetAbsolutePosition(m_vc, pos);
}

inline int Virtuose::beepOnLimit(int enable) {
	return virtBeepOnLimit(m_vc, enable);
}

inline int Virtuose::enableForceFeedback(int enable) {
	return virtEnableForceFeedback(m_vc, enable);
}

inline int Virtuose::getPhysicalSpeed(float * speed) {
	return virtGetPhysicalSpeed(m_vc, speed);
}

inline int Virtuose::forceShiftButton(int forceShiftButton) {
	return virtForceShiftButton(m_vc, forceShiftButton);
}

inline int Virtuose::addForce(float * force) {
	return virtAddForce(m_vc, force);
}

#endif // INCLUDED_vpp_h_GUID_0d0a89d6_fd37_447c_aa27_ebc289ddb935

