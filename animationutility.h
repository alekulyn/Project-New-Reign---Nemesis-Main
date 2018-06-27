#ifndef ANIMATIONUTILITY_H_
#define ANIMATIONUTILITY_H_

typedef std::unordered_map<std::string, int> id;

struct animationutility
{
	id eventid;
	id variableid;	
	std::vector<int> fixedStateID;
	int stateCountMultiplier;
	bool hasGroup = false;
	int optionMulti = -1;
	int animMulti = -1;
	int groupMulti = -1;
	int order;
	std::string multiOption;

	animationutility() {}
	animationutility(id eventid, id variableid, std::vector<int> stateID, int stateCountMultiplier, bool hasGroup, int optionMulti = -1, int animMulti = -1, std::string multiOption = "");
};

#endif