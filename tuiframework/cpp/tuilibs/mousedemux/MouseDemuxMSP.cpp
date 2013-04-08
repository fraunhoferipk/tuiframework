#include "MouseDemuxMSP.h"

static const char * mouseDataInTag = "m";
static const char * outDXTag = "dx";
static const char * outDYTag = "dy";
static const char * outDWheelTag = "dwheel";
static const char * outBtnLTag = "b1left";
static const char * outBtnRTag = "b2right";
static const char * outBtnMTag = "b3middle";
static const char * outBtn4Tag = "b4";
static const char * outBtn5Tag = "b5";

#define OUT_DX 0
#define OUT_DY 1
#define OUT_DW 2
#define OUT_BTNL 3
#define OUT_BTNR 4
#define OUT_BTNM 5
#define OUT_BTN4 6
#define OUT_BTN5 7

using namespace std;

namespace tuiframework {

IMSP * MouseDemuxMSP::createFunction(void * arg) {
    MSPConfig * config = static_cast<MSPConfig *>(arg);
    return new MouseDemuxMSP(*config);
}

static std::string typeName = "MouseDemuxMSP";

std::string MouseDemuxMSP::getMSPTypeName() {
    return typeName;
}


MouseDemuxMSP::MouseDemuxMSP(const MSPConfig & config) :
    config(config) {
	for (int i = 0; i < 8; ++i) {
		this->out[i] = 0;
	}
	this->mouseEventDelegate.setReceiver(this, &MouseDemuxMSP::handleMouseEvent);
	this->nameToIdMap[outDXTag] = OUT_DX;
	this->nameToIdMap[outDYTag] = OUT_DY;
	this->nameToIdMap[outDWheelTag] = OUT_DW;
	this->nameToIdMap[outBtnLTag] = OUT_BTNL;
	this->nameToIdMap[outBtnRTag] = OUT_BTNR;
	this->nameToIdMap[outBtnMTag] = OUT_BTNM;
	this->nameToIdMap[outBtn4Tag] = OUT_BTN4;
	this->nameToIdMap[outBtn5Tag] = OUT_BTN5;
}


MouseDemuxMSP::~MouseDemuxMSP() {
}


const std::string & MouseDemuxMSP::getTypeName() const {
	return getMSPTypeName();
}


IEventSink * MouseDemuxMSP::getEventSink(const std::string & name) {
	if (name.compare(mouseDataInTag) == 0) {
		return &mouseEventDelegate;
	}
	return 0;
}


void MouseDemuxMSP::registerEventSink(const std::string & name, IEventSink * eventSink) {
	map<string, int>::iterator iter = this->nameToIdMap.find(name);
	if (iter == this->nameToIdMap.end()) {
		return;
	}
	this->out[(*iter).second] = eventSink;
}


const MSPType & MouseDemuxMSP::getMSPType() const {
	return this->type;
}


void MouseDemuxMSP::handleMouseEvent(MouseEvent * e) {
	const MouseData & md = e->getPayload();

	if (this->out[OUT_DX]) {
		if (md.getDeltaX()) {
			this->out[OUT_DX]->push(new IntegerChangedEvent(-1, -1, md.getDeltaX()));
		}
	}
	if (this->out[OUT_DY]) {
		if (md.getDeltaY()) {
			this->out[OUT_DY]->push(new IntegerChangedEvent(-1, -1, md.getDeltaY()));
		}
	}
	if (this->out[OUT_DW]) {
		if (md.getDeltaWheel()) {
			this->out[OUT_DW]->push(new IntegerChangedEvent(-1, -1, md.getDeltaWheel()));
		}
	}
	if (this->out[OUT_BTNL]) {
		if (md.getButtonStateChange(MouseData::BUTTON_1_DOWN)) {
			this->out[OUT_BTNL]->push(new DigitalChangedEvent(-1, -1, true));
		}
		if (md.getButtonStateChange(MouseData::BUTTON_1_UP)) {
			this->out[OUT_BTNL]->push(new DigitalChangedEvent(-1, -1, false));
		}
	}

	if (this->out[OUT_BTNR]) {
		if (md.getButtonStateChange(MouseData::BUTTON_2_DOWN)) {
			this->out[OUT_BTNR]->push(new DigitalChangedEvent(-1, -1, true));
		}
		if (md.getButtonStateChange(MouseData::BUTTON_2_UP)) {
			this->out[OUT_BTNR]->push(new DigitalChangedEvent(-1, -1, false));
		}
	}

	if (this->out[OUT_BTNM]) {
		if (md.getButtonStateChange(MouseData::BUTTON_3_DOWN)) {
			this->out[OUT_BTNM]->push(new DigitalChangedEvent(-1, -1, true));
		}
		if (md.getButtonStateChange(MouseData::BUTTON_3_UP)) {
			this->out[OUT_BTNM]->push(new DigitalChangedEvent(-1, -1, false));
		}
	}

	if (this->out[OUT_BTN4]) {
		if (md.getButtonStateChange(MouseData::BUTTON_4_DOWN)) {
			this->out[OUT_BTN4]->push(new DigitalChangedEvent(-1, -1, true));
		}
		if (md.getButtonStateChange(MouseData::BUTTON_4_UP)) {
			this->out[OUT_BTN4]->push(new DigitalChangedEvent(-1, -1, false));
		}
	}

	if (this->out[OUT_BTN5]) {
		if (md.getButtonStateChange(MouseData::BUTTON_5_DOWN)) {
			this->out[OUT_BTN5]->push(new DigitalChangedEvent(-1, -1, true));
		}
		if (md.getButtonStateChange(MouseData::BUTTON_5_UP)) {
			this->out[OUT_BTN5]->push(new DigitalChangedEvent(-1, -1, false));
		}
	}

	delete e;
}



}