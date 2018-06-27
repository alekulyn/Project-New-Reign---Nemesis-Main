#include "animationsetdata.h"

using namespace std;

AnimationDataProject::AnimationDataProject(int& startline, vecstr& animdatafile, string filename)
{
	// data list
	DataPackProcess(datalist, startline, animdatafile, filename);
	++startline;

	for (auto it = datalist.begin(); it != datalist.end(); ++it)
	{
		++startline;

		if (startline >= int(animdatafile.size()))
		{
			ErrorMessage(5019, filename);
			throw 1;
		}

		// equip list
		EquipPackProcess(it->second.equiplist, startline, animdatafile, filename, it->first);

		// type list
		TypePackProcess(it->second.typelist, startline, animdatafile, filename, it->first);

		// anim list
		AnimPackProcess(it->second.animlist, startline, animdatafile, filename, it->first);

		// crc3 list
		CRC32Process(it->second.crc32list, startline, animdatafile, filename, it->first);
	}
}

void DataPackProcess(map<string, datapack, alphanum_less>& storeline, int& startline, vecstr& animdatafile, string filename)
{
	if (startline >= int(animdatafile.size()))
	{
		ErrorMessage(5018, filename, "Header");
		throw 1;
	}
	else if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(5001, filename, startline);
		throw 1;
	}

	for (int i = startline + 1; i < int(animdatafile.size()); ++i)
	{
		datapack newDataPack;
		storeline[animdatafile[i]] = newDataPack;

		if (i + 4 >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, "Header");
			throw 1;
		}

		if (boost::iequals(animdatafile[i + 1], "V3"))
		{
			startline = i;
			break;
		}
	}
}

void EquipPackProcess(std::vector<equip>& storeline, int& startline, vecstr& animdatafile, string filename, string header)
{
	if (startline >= int(animdatafile.size()))
	{
		ErrorMessage(5018, filename, header);
		throw 1;
	}
	else if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(5001, filename, startline);
		throw 1;
	}

	for (int i = startline + 1; i < int(animdatafile.size()); ++i)
	{
		if (isOnlyNumber(animdatafile[i]))
		{
			startline = i;
			break;
		}

		equip tempEquip;
		tempEquip.name = animdatafile[i];
		storeline.push_back(tempEquip);

		if (i + 1 >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
	}
}

void TypePackProcess(vector<typepack>& storeline, int& startline, vecstr& animdatafile, string filename, string header)
{
	if (startline >= int(animdatafile.size()))
	{
		ErrorMessage(5018, filename, header);
		throw 1;
	}
	else if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(5001, filename, header);
		throw 1;
	}

	int counter = 0;

	for (int i = startline + 1; i < int(animdatafile.size()); ++i)
	{
		if (isOnlyNumber(animdatafile[i]))
		{
			if (i + 1 < int(animdatafile.size()) && hasAlpha(animdatafile[i + 1]))
			{
				startline = i;
				break;
			}
			else if (i + 4 < int(animdatafile.size()) && animdatafile[i + 4] == "7891816")
			{
				startline = i;
				break;
			}
			else if (i + 2 < int(animdatafile.size()) && boost::iequals(animdatafile[i + 2], "V3"))
			{
				startline = i;
				break;
			}
			else if (i + 2 == animdatafile.size())
			{
				startline = i;
				break;
			}
		}

		typepack tempTP;

		if (isOnlyNumber(animdatafile[i]) || !hasAlpha(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempTP.name = animdatafile[i];

		if (++i >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
		else if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempTP.equiptype1 = animdatafile[i];
		
		if (++i >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
		else if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempTP.equiptype2 = animdatafile[i];
		storeline.push_back(tempTP);

		if (i + 1 >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
	}
}

void AnimPackProcess(vector<animpack>& storeline, int& startline, vecstr& animdatafile, string filename, string header)
{
	if (startline >= int(animdatafile.size()))
	{
		ErrorMessage(5018, filename, header);
		throw 1;
	}
	else if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(5001, filename, header);
		throw 1;
	}

	for (int i = startline + 1; i < int(animdatafile.size()); ++i)
	{
		if (animdatafile[i] == "0")
		{
			if (i + 1 < int(animdatafile.size()) && animdatafile[i + 1] == "V3")
			{
				startline = i;
				break;
			}
			else if (i + 2 < int(animdatafile.size()) && animdatafile[i + 2].find(".txt") != NOT_FOUND)
			{
				startline = i;
				break;
			}
		}
		
		if (animdatafile[i] == "1")
		{
			if (i + 4 < int(animdatafile.size()) && animdatafile[i + 4] == "V3")
			{
				startline = i;
				break;
			}
			else if (i + 5 < int(animdatafile.size()) && animdatafile[i + 5].find(".txt") != NOT_FOUND)
			{
				startline = i;
				break;
			}
		}
		
		if (isOnlyNumber(animdatafile[i]))
		{
			if (i + 3 < int(animdatafile.size()) && animdatafile[i + 3] == "7891816")
			{
				startline = i;
				break;
			}
			else if (i + 1 == animdatafile.size())
			{
				startline = i;
				break;
			}
		}

		animpack tempAP;
		tempAP.name = animdatafile[i++];
		tempAP.unknown = animdatafile[i++];

		if (i >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
		else if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}
		
		while (i < int(animdatafile.size()))
		{
			++i;

			if (animdatafile[i] == "0")
			{
				if (i + 1 < int(animdatafile.size()) && animdatafile[i + 1] == "V3")
				{
					--i;
					break;
				}
				else if (i + 2 < int(animdatafile.size()) && animdatafile[i + 2].find(".txt") != NOT_FOUND)
				{
					--i;
					break;
				}
			}
			
			if (animdatafile[i] == "1")
			{
				if (i + 4 < int(animdatafile.size()) && animdatafile[i + 4] == "V3")
				{
					--i;
					break;
				}
				else if (i + 5 < int(animdatafile.size()) && animdatafile[i + 5].find(".txt") != NOT_FOUND)
				{
					--i;
					break;
				}
			}
			
			if (isOnlyNumber(animdatafile[i]))
			{
				if (i + 3 < int(animdatafile.size()) && animdatafile[i + 3] == "7891816")
				{
					--i;
					break;
				}
				else if (i + 1 == animdatafile.size())
				{
					startline = i;
					break;
				}
			}
			
			if (hasAlpha(animdatafile[i]))
			{
				bool out = false;

				for (int k = i + 1; k < i + 3; ++k)
				{
					if (!isOnlyNumber(animdatafile[k]))
					{
						out = false;
						break;
					}
					else
					{
						out = true;
					}
				}

				if (out)
				{
					if (i + 3 < int(animdatafile.size()) && hasAlpha(animdatafile[i + 3]) && animdatafile[i + 3] != "V3")
					{
						--i;
						break;
					}
				}
			}

			attackdata attack;
			tempAP.attack.push_back(attack);
			tempAP.attack.back().data = animdatafile[i];
		}

		storeline.push_back(tempAP);

		if (i + 1 >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
	}
}

void CRC32Process(vector<crc32>& storeline, int& startline, vecstr& animdatafile, string filename, string header)
{
	if (startline >= int(animdatafile.size()))
	{
		ErrorMessage(5018, filename, header);
		throw 1;
	}
	else if (!isOnlyNumber(animdatafile[startline]))
	{
		ErrorMessage(5001, filename, header);
		throw 1;
	}

	for (int i = startline + 1; i < int(animdatafile.size()); ++i)
	{
		if (animdatafile[i] == "V3")
		{
			startline = i;
			break;
		}
		else if (i + 1 < int(animdatafile.size()) && animdatafile[i + 1].find(".txt") != NOT_FOUND)
		{
			startline = i;
			break;
		}

		crc32 tempCRC32;

		if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempCRC32.filepath = animdatafile[i];

		if (++i >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
		else if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempCRC32.filename = animdatafile[i];

		if (++i >= int(animdatafile.size()))
		{
			ErrorMessage(5018, filename, header);
			throw 1;
		}
		else if (!isOnlyNumber(animdatafile[i]))
		{
			ErrorMessage(5001, filename, header);
			throw 1;
		}

		tempCRC32.fileformat = animdatafile[i];
		storeline.push_back(tempCRC32);
		
		if (i + 1 >= int(animdatafile.size()))
		{
			startline = i;
		}
	}
}

ASDFormat::position ASDPosition(vecstr animData, string project, string header, string modcode, int linecount, bool muteError)
{
	// has function
	bool isOpen = false;
	int functionstart = -1;
	int conditionOpen = 0;
	unordered_map<int, bool> isCondition;
	unordered_map<int, bool> isConditionOri;
	unordered_map<int, ASDFunct> marker;

	if (linecount >= int(animData.size()))
	{
		ErrorMessage(5010, modcode, project, header);
		return ASDFormat::xerror;
	}

	if (animData[linecount].find("<!-- ") != NOT_FOUND)
	{
		if (!muteError)
		{
			ErrorMessage(3007, modcode, "animationsetdatasinglefile.txt", linecount, header);
			return ASDFormat::xerror;
		}
	}

	for (unsigned int i = 0; i < animData.size(); ++i)
	{
		if (animData[i].find("<!-- ") != NOT_FOUND)
		{
			if (functionstart == -1)
			{
				functionstart = i;
			}

			marker[i].skip = true;

			if (animData[i].find("<!-- CLOSE -->") != NOT_FOUND)
			{
				if (!isOpen)
				{
					ErrorMessage(1171, modcode, project + "\\" + header, i + 1);
					return ASDFormat::xerror;
				}

				isOpen = false;
			}
			else if (animData[i].find("<!-- CONDITION END -->") != NOT_FOUND)
			{
				isCondition[conditionOpen] = false;
				isConditionOri[conditionOpen] = false;
				--conditionOpen;
			}
		}

		if (isOpen)
		{
			marker[i].isNew = true;
		}

		if (isCondition[conditionOpen])
		{
			marker[i].isCondition = true;
			marker[i].conditionOpen = conditionOpen;

		}
		else if (isConditionOri[conditionOpen])
		{
			marker[i].isConditionOri = true;
			marker[i].conditionOpen = conditionOpen;
		}

		if (animData[i].find("<!-- ") != NOT_FOUND)
		{
			if (animData[i].find("<!-- NEW") != NOT_FOUND)
			{
				if (isOpen)
				{
					ErrorMessage(1115, modcode, project + "\\" + header, i + 1);
					return ASDFormat::xerror;
				}

				isOpen = true;
			}
			else if (animData[i].find("<!-- CONDITION START ^") != NOT_FOUND)
			{
				++conditionOpen;
				isCondition[conditionOpen] = true;
			}
			else if (animData[i].find("<!-- CONDITION -->") != NOT_FOUND)
			{
				isCondition[conditionOpen] = false;
				isConditionOri[conditionOpen] = true;
			}
			else if (animData[i].find("<!-- CONDITION ^") != NOT_FOUND)
			{
				marker[i].nextCondition = true;
			}
		}
	}

	using namespace ASDFormat;

	if (linecount < 2 && functionstart < 1)
	{
		return ASDConvert(linecount + 1, muteError);
		// 1. V3
		// 2. equipcount
	}
	else
	{
		int id = 0;

		try
		{
			int curline = 0;
			int result = PositionLineCondition(curline, 0, linecount, animData, marker, modcode, header, true, muteError);

			if (result == -2)
			{
				if (!muteError)
				{
					ErrorMessage(5008, modcode, project, header);
				}

				return xerror;
			}
		}
		catch (double curID)
		{
			id = static_cast<int>(curID);
		}

		return ASDConvert(id, muteError);
	}

	if (!muteError)
	{
		ErrorMessage(5009, project, header);
	}

	return xerror;
}

ASDFormat::position ASDConvert(int position, bool muteError)
{
	using namespace ASDFormat;

	if (position == 1)
	{
		return V3;
	}
	else if (position == 2)
	{
		return equipcount;
	}
	else if (position == 3)
	{
		return equiplist;
	}
	else if (position == 4)
	{
		return typecount;
	}
	else if (position == 5)
	{
		return typelist;
	}
	else if (position == 6)
	{
		return animpackcount;
	}
	else if (position == 7)
	{
		return animpackname;
	}
	else if (position == 8)
	{
		return unknown3;
	}
	else if (position == 9)
	{
		return attackcount;
	}
	else if (position == 10)
	{
		return attacklist;
	}
	else if (position == 11)
	{
		return crc32count;
	}
	else if (position == 12)
	{
		return crc32list;
	}
	else if (position == 13)
	{
		return unknown1;
	}
	else if (position == 14)
	{
		return unknown2;
	}

	return xerror;
}

int PositionLineCondition(int& i, double curID, int linecount, vecstr animDataSet, unordered_map<int, ASDFunct>& marker, string modcode, string header, bool last, bool muteError)
{
	using namespace ASDFormat;
	double id = curID;
	int conditionOpen = marker[i].conditionOpen;
	bool jump = false;
	
	if (id < 3)
	{
		while (i < linecount + 1)
		{
			if (!marker[i].skip)
			{
				if (id == 2)
				{
					++id;
					break;
				}

				if (marker[i].isCondition)		// new condition
				{
					if (conditionOpen < marker[i].conditionOpen)
					{
						int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

						if (exit == -1 && !last)
						{
							return -1;
						}
					}
					else if (conditionOpen > marker[i].conditionOpen)
					{
						--i;
						return 0;
					}
					else
					{
						++id;
					}
				}
				else
				{
					if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
					{
						--i;
						return 0;
					}
					else
					{
						if (!last)
						{
							--i;
							return -1;
						}
						else
						{
							++id;
						}
					}
				}

				if (marker[i].isNew)
				{
					if (!muteError)
					{
						ErrorMessage(5007, modcode, i + 1, header);
					}

					return -1;
				}
			}
			else if (marker[i].nextCondition)		// next condition or close condition
			{
				id = curID;
			}

			++i;
		}
	}
	
	if (id == 3)
	{
		while (i < linecount + 1)
		{
			if (!marker[i].skip)
			{
				if (isOnlyNumber(animDataSet[i]))
				{
					++id;
					break;
				}

				if (marker[i].isCondition)		// new condition
				{
					if (conditionOpen < marker[i].conditionOpen)
					{
						int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

						if (exit == -1 && !last)
						{
							return -1;
						}
					}
					else if (conditionOpen > marker[i].conditionOpen)
					{
						--i;
						return 0;
					}
				}
				else
				{
					if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
					{
						--i;
						return 0;
					}
					else
					{
						if (!last)
						{
							--i;
							return -1;
						}
					}
				}

				if (marker[i].isNew)
				{
					if (id != 3)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, linecount);
						}

						return -1;
					}
				}
			}
			else if (marker[i].nextCondition)		// next condition or close condition
			{
				id = curID;
			}

			++i;
		}
	}

	if (id == 4)
	{
		if (isOnlyNumber(animDataSet[i]) && animDataSet[i] == "0")
		{
			if (i == linecount)
			{
				throw id;
			}

			++id;
			++id;
			++i;
		}
		else
		{
			while (i < linecount + 1)
			{
				if (!marker[i].skip)
				{
					if (hasAlpha(animDataSet[i]))
					{
						jump = true;
						++id;
						break;
					}

					if (marker[i].isCondition)		// new condition
					{
						if (conditionOpen < marker[i].conditionOpen)
						{
							int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

							if (exit == -1 && !last)
							{
								return -1;
							}
						}
						else if (conditionOpen > marker[i].conditionOpen)
						{
							--i;
							return 0;
						}
					}
					else
					{
						if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
						{
							--i;
							return 0;
						}
						else
						{
							if (!last)
							{
								--i;
								return -1;
							}
						}
					}

					if (marker[i].isNew)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, i + 1, header);
						}

						return -1;
					}
				}
				else if (marker[i].nextCondition)		// next condition or close condition
				{
					id = curID;
				}

				++i;
			}
		}
	}

	if (id == 5 || id == 13 || id == 14)		// change this
	{
		if (id == 5 && jump)
		{
			jump = false;
			id = 14;
		}

		while (i < linecount + 1)
		{
			if (!marker[i].skip)
			{
				if (isOnlyNumber(animDataSet[i]) && id == 14)
				{
					if (animDataSet[i] == "0")
					{
						if (id == 14)
						{
							id = 6;
							break;
						}
					}
					else
					{
						int next = 1;

						if (animDataSet[i + next].find("<!--") != NOT_FOUND)
						{
							++next;
						}

						if (hasAlpha(animDataSet[i + next]))
						{
							++next;

							if (animDataSet[i + next].find("<!--") != NOT_FOUND)
							{
								++next;
							}

							if (isOnlyNumber(animDataSet[i + next]))
							{
								++next;

								if (animDataSet[i + next].find("<!--") != NOT_FOUND)
								{
									++next;
								}

								if (isOnlyNumber(animDataSet[i + next]))
								{
									++next;

									if (animDataSet[i + next].find("<!--") != NOT_FOUND)
									{
										++next;
									}

									if (hasAlpha(animDataSet[i + next]))
									{
										id = 6;
										break;
									}
								}
							}
						}
					}
				}

				if (marker[i].isCondition)		// new condition
				{
					if (conditionOpen < marker[i].conditionOpen)
					{
						int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

						if (exit == -1 && !last)
						{
							return -1;
						}
					}
					else if (conditionOpen > marker[i].conditionOpen)
					{
						--i;
						return 0;
					}
					else
					{
						if (id == 14)
						{
							if (hasAlpha(animDataSet[i]))
							{
								id = 5;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else if (id == 5)
						{
							if (isOnlyNumber(animDataSet[i]))
							{
								id = 13;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else if (id == 13)
						{
							if (isOnlyNumber(animDataSet[i]))
							{
								id = 14;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else
						{
							if (!muteError)
							{
								ErrorMessage(5007, modcode, i + 1, header);
							}

							return -1;
						}
					}
				}
				else
				{
					if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
					{
						--i;
						return 0;
					}
					else
					{
						if (!last)
						{
							--i;
							return -1;
						}
						else
						{
							if (id == 14)
							{
								if (hasAlpha(animDataSet[i]))
								{
									id = 5;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else if (id == 5)
							{
								if (isOnlyNumber(animDataSet[i]))
								{
									id = 13;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else if (id == 13)
							{
								if (isOnlyNumber(animDataSet[i]))
								{
									id = 14;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
					}
				}

				if (marker[i].isNew)
				{
					if (id != 5 && id != 13 && id != 14)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, linecount);
						}

						return -1;
					}
				}
			}
			else if (marker[i].nextCondition)		// next condition or close condition
			{
				id = curID;
			}

			++i;
		}
	}

	if (id == 6)
	{
		if (isOnlyNumber(animDataSet[i]) && animDataSet[i] == "0")
		{
			if (i == linecount)
			{
				throw id;
			}

			id = 11;
			++i;
		}
		else
		{
			while (i < linecount + 1)
			{
				if (!marker[i].skip)
				{
					if (hasAlpha(animDataSet[i]))
					{
						jump = true;
						++id;
						break;
					}

					if (marker[i].isCondition)		// new condition
					{
						if (conditionOpen < marker[i].conditionOpen)
						{
							int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

							if (exit == -1 && !last)
							{
								return -1;
							}
						}
						else if (conditionOpen > marker[i].conditionOpen)
						{
							--i;
							return 0;
						}
						else
						{
							if (id != 6)
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
					}
					else
					{
						if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
						{
							--i;
							return 0;
						}
						else
						{
							if (!last)
							{
								--i;
								return -1;
							}
							else
							{
								if (id != 6)
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
						}
					}

					if (marker[i].isNew)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, linecount);
						}

						return -1;
					}
				}
				else if (marker[i].nextCondition)		// next condition or close condition
				{
					id = curID;
				}

				++i;
			}
		}
	}

	if (id > 6 && id < 11)
	{
		if (id == 7 && jump)
		{
			jump = false;
			id = 10;
		}

		while (i < linecount + 1)
		{
			if (!marker[i].skip)
			{
				if (isOnlyNumber(animDataSet[i]) && id == 10)
				{
					if (animDataSet[i] == "0" && (i == int(animDataSet.size()) - 1 || boost::iequals(animDataSet[i + 1], "V3")))
					{
						++id;
						break;
					}
					else
					{
						int next = 1;

						if (animDataSet[i + next].find("<!--") != NOT_FOUND)
						{
							++next;
						}

						if (isOnlyNumber(animDataSet[i + next]))
						{
							++next;

							if (animDataSet[i + next].find("<!--") != NOT_FOUND)
							{
								++next;
							}

							if (isOnlyNumber(animDataSet[i + next]))
							{
								++next;

								if (animDataSet[i + next].find("<!--") != NOT_FOUND)
								{
									++next;
								}

								if (isOnlyNumber(animDataSet[i + next]) && animDataSet[i + next] == "7891816")
								{
									++id;
									break;
								}
							}
						}
					}
				}

				if (marker[i].isCondition)		// new condition
				{
					if (conditionOpen < marker[i].conditionOpen)
					{
						int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

						if (exit == -1 && !last)
						{
							return -1;
						}
					}
					else if (conditionOpen > marker[i].conditionOpen)
					{
						--i;
						return 0;
					}
					else
					{
						if (id == 10)
						{
							if (hasAlpha(animDataSet[i]))
							{
								int next = i + 1;

								while (true)
								{
									if (marker[next].skip)
									{
										++next;
									}
									else
									{
										break;
									}
								}

								if (isOnlyNumber(animDataSet[next]))
								{
									id = 7;
								}
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else if (id == 7)
						{
							if (isOnlyNumber(animDataSet[i]))
							{
								id = 8;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else if (id == 8)
						{
							if (isOnlyNumber(animDataSet[i]))
							{
								id = 9;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else if (id == 9)
						{
							if (hasAlpha(animDataSet[i]))
							{
								id = 10;
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
						else
						{
							if (!muteError)
							{
								ErrorMessage(5007, modcode, i + 1, header);
							}

							return -1;
						}
					}
				}
				else
				{
					if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
					{
						--i;
						return 0;
					}
					else
					{
						if (!last)
						{
							--i;
							return -1;
						}
						else
						{
							if (id == 10)
							{
								if (hasAlpha(animDataSet[i]))
								{
									int next = i + 1;

									while (true)
									{
										if (marker[next].skip)
										{
											++next;
										}
										else
										{
											break;
										}
									}

									if (isOnlyNumber(animDataSet[next]))
									{
										id = 7;
									}
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else if (id == 7)
							{
								if (isOnlyNumber(animDataSet[i]))
								{
									id = 8;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else if (id == 8)
							{
								if (isOnlyNumber(animDataSet[i]))
								{
									id = 9;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else if (id == 9)
							{
								if (hasAlpha(animDataSet[i]))
								{
									id = 10;
								}
								else
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
							else
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
					}
				}

				if (marker[i].isNew)
				{
					if (id != 7 && id != 8 && id != 9 && id != 10)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, linecount);
						}

						return -1;
					}
				}
			}
			else if (marker[i].nextCondition)		// next condition or close condition
			{
				id = curID;
			}

			++i;
		}
	}

	if (id == 11)
	{
		if (isOnlyNumber(animDataSet[i]) && animDataSet[i] == "0")
		{
			if (i == linecount)
			{
				throw id;
			}

			++id;
		}
		else
		{
			while (i < linecount + 1)
			{
				if (i + 3 < int(animDataSet.size()) && isOnlyNumber(animDataSet[i]))
				{
					int next = 1;

					if (animDataSet[i + next].find("<!--") != NOT_FOUND)
					{
						++next;
					}

					if (isOnlyNumber(animDataSet[i + next]))
					{
						++next;

						if (animDataSet[i + next].find("<!--") != NOT_FOUND)
						{
							++next;
						}

						if (animDataSet[i + next] == "7891816")
						{
							jump = true;
							++id;
							break;
						}
					}
				}

				if (!marker[i].skip)
				{
					if (marker[i].isCondition)		// new condition
					{
						if (conditionOpen < marker[i].conditionOpen)
						{
							int exit = PositionLineCondition(i, id, linecount, animDataSet, marker, modcode, header, false, muteError);

							if (exit == -1 && !last)
							{
								return -1;
							}
						}
						else if (conditionOpen > marker[i].conditionOpen)
						{
							--i;
							return 0;
						}
						else
						{
							if (id != 11)
							{
								if (!muteError)
								{
									ErrorMessage(5007, modcode, i + 1, header);
								}

								return -1;
							}
						}
					}
					else
					{
						if (marker[i].isConditionOri && marker[i].conditionOpen != conditionOpen + 1)	// original condition
						{
							--i;
							return 0;
						}
						else
						{
							if (!last)
							{
								--i;
								return -1;
							}
							else
							{
								if (id != 11)
								{
									if (!muteError)
									{
										ErrorMessage(5007, modcode, i + 1, header);
									}

									return -1;
								}
							}
						}
					}

					if (marker[i].isNew)
					{
						if (!muteError)
						{
							ErrorMessage(5007, modcode, linecount);
						}

						return -1;
					}
				}
				else if (marker[i].nextCondition)		// next condition or close condition
				{
					id = curID;
				}

				++i;
			}
		}
	}

	if (id == 12)
	{
		if (!isOnlyNumber(animDataSet[i]))
		{
			if (!muteError)
			{
				ErrorMessage(5007, modcode, i + 1, header);
			}

			return -1;
		}
	}

	if (id < 15)
	{
		throw id;
	}

	return -2;
}

void combineExtraction(vecstr& storeline, map<int, vecstr> extract, string project, string header)
{
	vecstr newline;
	bool newOpen = false;
	int condition = 0;

	for (unsigned int i = 0; i < storeline.size(); ++i)
	{
		string line = storeline[i];

		if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
		{
			condition++;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", 0) != NOT_FOUND)
		{
			newOpen = true;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ +% -->", 0) != NOT_FOUND)
		{
			newOpen = true;
		}
		else if (line.find("<!-- NEW ORDER ", 0) != NOT_FOUND && line.find(" -->", 0) != NOT_FOUND)
		{
			newOpen = true;
		}

		if (condition == 0 && !newOpen)
		{
			newline.push_back(storeline[i]);
		}

		if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
		{
			newOpen = false;
		}
		else if (line.find("<!-- CONDITION END -->", 0) != NOT_FOUND)
		{
			condition--;
		}

		if (extract[i].size() > 0)
		{
			if (condition == 0 && !newOpen)
			{
				ErrorMessage(5013, project, header);
				return;
			}

			for (unsigned int k = 0; k < extract[i].size(); ++k)
			{
				newline.push_back(extract[i][k]);
			}
		}
	}

	storeline = newline;
}
