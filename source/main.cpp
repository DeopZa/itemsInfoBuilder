#include <cstdio>
#include <atomic>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include "json.hpp"

using namespace std;

std::atomic<int> doneParsing;
int threadAmount = 10;

enum Mods {
	djump, //Double Jump
	hjump, //High Jump
	speedy, //Speedy
	fastdig, //Enhanced Digging
	fireproof, //Fireproof
	slowfall, //Slow-Fall
	xpbuff //XP Buff
};

struct item{
    //needed by GTace
    int mods = 0;
    string description = "No info.";

    //from items.dat
    int itemID = 0;
	char editableType = 0;
	char itemCategory = 0;
	unsigned char actionType = 0;
	char hitSoundType = 0;
	string name = "";
	string texture = "";
	int textureHash = 0;
	char itemKind = 0;
	int val1;
	char textureX = 0;
	char textureY = 0;
	char spreadType = 0;
	char isStripeyWallpaper = 0;
	char collisionType = 0;
	unsigned char breakHits = 0;
	int restoreTime = 0;
	char clothingType = 0;
	int16_t rarity = 0;
	unsigned char maxAmount = 0;
	string extraFile = "";
	int extraFileHash = 0;
    int IOSextraFileHash = 0;
	int audioVolume = 0;
	string petName = "";
	string petPrefix = "";
	string petSuffix = "";
	string petAbility = "";
	char seedBase = 0;
	char seedOverlay = 0;
	char treeBase = 0;
	char treeLeaves = 0;
	int16_t ing1 = 0;
	int16_t ing2 = 0;
	int seedColor = 0;
	int seedOverlayColor = 0;
	int growTime = 0;
	short val2;
	short isRayman = 0;
	string extraOptions = "";
	string texture2 = "";
	string extraOptions2 = "";
	string punchOptions = "";
	string extraFieldUnk_4 = "";
	short value = 0;
	short value2 = 0;
	short unkValueShort1 = 0;
	short unkValueShort2 = 0;
	int newValue = 0;
	char newValue1 = 0;
	char newValue2 = 0;
	char newValue3 = 0;
	char newValue4 = 0;
	char newValue5 = 0;
	char newValue6 = 0;
	char newValue7 = 0;
	char newValue8 = 0;
	char newValue9 = 0;
	int newInt1 = 0;
	int newInt2 = 0;
};

item* items = NULL;
int itemCount = 0;

void decode_itemsDat(){
    printf("Decoding items.dat\n");
    std::ifstream file("items.dat", std::ios::binary | std::ios::ate);
    int size = file.tellg();
    if (size == -1) {
        cout << "Didn't find items.dat. Is it at right place?" << endl;
        exit(-1);
    }
    char* data = new char[size];
    file.seekg(0, std::ios::beg);
    if (file.read((char*)(data), size));
    else {
        cout << "Something gone wrong while decoding .dat file!" << endl;
        exit(-1);
    }
    string secret = "PBG892FXX982ABC*";
    int memPos = 0;
    int itemsdatVer = 0;
	memcpy(&itemsdatVer, data + memPos, 2);
	memPos += 2;
	memcpy(&itemCount, data + memPos, 4);
	memPos += 4;
    items = new item[itemCount];
    if (itemsdatVer > 14){
        cout << "Unsupported items.dat! Version: " << to_string(itemsdatVer) << endl;
        exit(-1);
    }
    for (int i = 0; i < itemCount; i++) {
        item item;
		{
			memcpy(&item.itemID, data + memPos, 4);
			memPos += 4;
		}
		{
			item.editableType = data[memPos];
			memPos += 1;
		}
		{
			item.itemCategory = data[memPos];
			memPos += 1;
		}
		{
			item.actionType = data[memPos];
			memPos += 1;
		}
		{
			item.hitSoundType = data[memPos];
			memPos += 1;
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.name += data[memPos] ^ (secret[(j + item.itemID) % secret.length()]);
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.texture += data[memPos];
				memPos++;
			}
		}
		memcpy(&item.textureHash, data + memPos, 4);
		memPos += 4;
		item.itemKind = data[memPos];
		memPos += 1;
		memcpy(&item.val1, data + memPos, 4);
		memPos += 4;
		item.textureX = data[memPos];
		memPos += 1;
		item.textureY = data[memPos];
		memPos += 1;
		item.spreadType = data[memPos];
		memPos += 1;
		item.isStripeyWallpaper = data[memPos];
		memPos += 1;
		item.collisionType = data[memPos];
		memPos += 1;
		item.breakHits = data[memPos];
		memPos += 1;
		memcpy(&item.restoreTime, data + memPos, 4);
		memPos += 4;
		item.clothingType = data[memPos];
		memPos += 1;
		memcpy(&item.rarity, data + memPos, 2);
		memPos += 2;
		item.maxAmount = data[memPos];
		memPos += 1;
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.extraFile += data[memPos];
				memPos++;
			}
		}
		memcpy(&item.extraFileHash, data + memPos, 4);
		memPos += 4;
		memcpy(&item.audioVolume, data + memPos, 4);
		memPos += 4;
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.petName += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.petPrefix += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.petSuffix += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.petAbility += data[memPos];
				memPos++;
			}
		}
		{
			item.seedBase = data[memPos];
			memPos += 1;
		}
		{
			item.seedOverlay = data[memPos];
			memPos += 1;
		}
		{
			item.treeBase = data[memPos];
			memPos += 1;
		}
		{
			item.treeLeaves = data[memPos];
			memPos += 1;
		}
		{
			memcpy(&item.seedColor, data + memPos, 4);
			memPos += 4;
		}
		{
			memcpy(&item.seedOverlayColor, data + memPos, 4);
			memPos += 4;
		}
		memPos += 4; // deleted ingredients
		{
			memcpy(&item.growTime, data + memPos, 4);
			memPos += 4;
		}
		memcpy(&item.val2, data + memPos, 2);
		memPos += 2;
		memcpy(&item.isRayman, data + memPos, 2);
		memPos += 2;
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.extraOptions += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.texture2 += data[memPos];
				memPos++;
			}
		}
		{
			int16_t strLen = *(int16_t*)&data[memPos];
			memPos += 2;
			for (int j = 0; j < strLen; j++) {
				item.extraOptions2 += data[memPos];
				memPos++;
			}
		}
		//memPos += 80;
		{
			int16_t extraField4Length = 0;
			memcpy(&extraField4Length, data + memPos, 2);
			memPos += 2;
			for (int j = 0; j < extraField4Length; j++) {
				item.extraFieldUnk_4 += data[memPos];
				memPos++;
			}
			memPos += 4;
			memcpy(&item.value, data + memPos, 2);
			memPos += 2;
			memcpy(&item.value2, data + memPos, 2);
			memPos += 2;
			memcpy(&item.unkValueShort1, data + memPos, 2);
			//if (unkValueShort1 & 4) cout << itemID << ":Transmute" << endl;
			memPos += 2 + (16 - item.value);
			memcpy(&item.unkValueShort2, data + memPos, 2);
			memPos += 2;
		}
		memPos += 50;
		if (itemsdatVer >= 11) {
			{
				int16_t strLen = *(int16_t*)&data[memPos];
				memPos += 2;
				for (int j = 0; j < strLen; j++) {
					item.punchOptions += data[memPos];
					memPos++;
				}
			}
		}
        if (itemsdatVer >= 12){
			memcpy(&item.newValue, data + memPos, 4);
			memPos += 4;
			item.newValue1 = data[memPos];
			memPos++;
			item.newValue2 = data[memPos];
			memPos++;
			item.newValue3 = data[memPos];
			memPos++;
			item.newValue4 = data[memPos];
			memPos++;
			item.newValue5 = data[memPos];
			memPos++;
			item.newValue6 = data[memPos];
			memPos++;
			item.newValue7 = data[memPos];
			memPos++;
			item.newValue8 = data[memPos];
			memPos++;
			item.newValue9 = data[memPos];
			memPos++;
        }
		if (itemsdatVer >= 13){
			memcpy(&item.newInt1, data + memPos, 4);
			memPos += 4;
		}
		if (itemsdatVer >= 14){
			memcpy(&item.newInt2, data + memPos, 4);
			memPos += 4;
		}
        if (i != item.itemID) {
            cout << "Unordered item! Something gone wrong?" << endl;
            exit(-1);
        }
        items[i] = item;
    }
    printf("Decoded items.dat, size: %i.\n", itemCount);
}

vector<string> explode(const string& delimiter, const string& str)
{
	vector<string> arr;

	int strleng = str.length();
	int delleng = delimiter.length();
	if (delleng == 0)
		return arr;//no change

	int i = 0;
	int k = 0;
	while (i < strleng)
	{
		int j = 0;
		while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
			j++;
		if (j == delleng)//found delimiter
		{
			arr.push_back(str.substr(k, i - k));
			i += delleng;
			k = i;
		}
		else
		{
			i++;
		}
	}
	arr.push_back(str.substr(k, i - k));
	return arr;
}

void parseWiki(int threadNum){
    httplib::Client cli("https://growtopia.fandom.com");
    for (int i = threadNum * 2; i < itemCount; i += threadAmount * 2){
        if (items[i].name.substr(0, 9) == "null_item") continue;
        string name = items[i].name;
        if (name.find("?") != string::npos) name.replace(name.find("?"), 1, "%3F");
        string path = "/wiki/" + name + "?action=raw";
        auto res = cli.Get(path.c_str());
        if (res->status != 200) {
            printf("Server returned status %i for %s!\n", res->status, items[i].name.c_str());
            continue;
        }
        string response = res->body;
        if (response.find("#redirect [[") != string::npos || response.find("#REDIRECT [[") != string::npos || response.find("#redirect[[") != string::npos || response.find("#REDIRECT[[") != string::npos){
            int foundlen = 12;
            size_t firstl = response.find("#redirect [[");
            if (firstl == string::npos) firstl = response.find("#REDIRECT [[");
            if (firstl == string::npos) {
                foundlen = 11;
                firstl = response.find("#redirect[[");
            }
            if (firstl == string::npos) {
                foundlen = 11;
                firstl = response.find("#REDIRECT[[");
            }
            size_t secondl = response.find("]]", firstl);
            string redirect_to = response.substr(firstl + foundlen, secondl - firstl - foundlen);
            if (redirect_to.find("#") != string::npos) redirect_to = redirect_to.substr(0, redirect_to.find("#"));
            path = "/wiki/" + redirect_to + "?action=raw";
            res = cli.Get(path.c_str());
            if (res->status != 200) {
                printf("Server returned status %i for %s (redirect from: %s)!\n", res->status, redirect_to.c_str(), items[i].name.c_str());
                continue;
            }
            response = res->body;
            while (response.find("{{PAGENAME}}") != string::npos) response.replace(response.find("{{PAGENAME}}"), 12, redirect_to);
            while (response.find("Item/ByName") != string::npos){
                size_t firstl = response.find("{{Item/ByName");
                size_t secondl = response.find("}}", firstl);
                string result = response.substr(firstl + 2, secondl - firstl - 2);
				while (result.find("[[") != string::npos){//remove links to other items
					size_t firstl = result.find("[[");
					size_t secondl = result.find("]]");
					string replace = result.substr(firstl, secondl - firstl + 2);
					string link = replace.substr(2, replace.length() - 4);
					vector<string> opts = explode("|", link);
					if (opts.size() >= 2){
						string name = opts[1];
						result.replace(firstl, secondl - firstl + 2, name);
					}
					else break;
				}
                vector<string> info = explode("|", result);
                if (info[0] == "Item/ByName"){
                    if (info[1].substr(0, 4) == "name") {
                        if (info[1].substr(5) == items[i].name || info[1].substr(7) == items[i].name){
                            items[i].description = info[2];
                            if (items[i].description.find("\n") != string::npos) name.replace(name.find("\n"), 1, "<CR>");
                        }
                    } else if (info.size() == 4) {
                        if (info[3].substr(5) == items[i].name || info[3].substr(7) == items[i].name) {
                            items[i].description = info[1];
                            if (items[i].description.find("\n") != string::npos) name.replace(name.find("\n"), 1, "<CR>");
                        }
                    } else if (info.size() == 3) {
                        if (info[2].substr(5) == items[i].name || info[2].substr(7) == items[i].name) {
                            items[i].description = info[1];
                            if (items[i].description.find("\n") != string::npos) name.replace(name.find("\n"), 1, "<CR>");
                        }
                    }
                }
                response = response.substr(secondl, response.length());
            }
            if (items[i].description != "No info.") continue;
        }
        size_t firstl = response.find("{{Item");
        size_t secondl = response.find("}}", firstl);
        if (firstl != string::npos && secondl != string::npos){
            string result = response.substr(firstl + 2, secondl - firstl - 2);
			while (result.find("[[") != string::npos){//remove links to other items
				size_t firstl = result.find("[[");
				size_t secondl = result.find("]]");
				string replace = result.substr(firstl, secondl - firstl + 2);
				string link = replace.substr(2, replace.length() - 4);
				vector<string> opts = explode("|", link);
				if (opts.size() >= 2){
					string name = opts[1];
					result.replace(firstl, secondl - firstl + 2, name);
				}
				else break;
			}
            vector<string> info = explode("|", result);
            if (info[0] == "Item" && info.size() >= 2) {
                if (info[1] == "No info.") continue;
                items[i].description = info[1];
                //add replace \n to <CR>
            } else if (info[0] == "Item/ByName"){
                if (info[1] == "No info.") continue;
                if (info[1].substr(0, 4) == "name"){
                    if (info[1].substr(5) == items[i].name || info[1].substr(7) == items[i].name){
                        items[i].description = info[2];
                        if (items[i].description.find("\n") != string::npos) name.replace(name.find("\n"), 1, "<CR>");
                    }
                } else if (info.size() == 4) {
                    if (info[3].substr(5) == items[i].name || info[3].substr(7) == items[i].name) {
                        items[i].description = info[1];
                        if (items[i].description.find("\n") != string::npos) name.replace(name.find("\n"), 1, "<CR>");
                    }
                } else if (info.size() == 3) {
                    if (info[2].substr(5) == items[i].name || info[2].substr(7) == items[i].name) {
                        items[i].description = info[1];
                        if (items[i].description.find("\n") != string::npos) name.replace(name.find("\n"), 1, "<CR>");
                    }
                }
            }
        }
        if (items[i].description == "No info.") printf("Didn't find description for %s (id: %i)\n", items[i].name.c_str(), i);
    }
    doneParsing++;
}

void parseWiki_mods(){
    httplib::Client cli("https://growtopia.fandom.com");
    auto res = cli.Get("/wiki/Mods?action=raw");
    if (res->status != 200) {
        printf("Server returned status %i while trying to parse mods.\n", res->status);
        return;
    }
    string response = res->body;
    int firstl = response.find("<tabber>");
    response = response.substr(firstl + 9, response.find("</tabber>") - firstl - 9);
    struct Mod{
        string mod = "";
        vector<string> items;
    };
    vector<Mod> mods;
    vector<string> work = explode("|-|", response);
    for (string& s : work){
        Mod mod;
        mod.mod = s.substr(0, s.find("="));
        int index = 0;
        while (s.find("{{ItemLink|", index) != string::npos) {
            int firstl = s.find("{{ItemLink|", index);
            mod.items.push_back(s.substr(firstl + 11, s.find("}}", firstl) - firstl - 11));
            index = firstl + 11;
        }
        mods.push_back(mod);
    }
	for (int i = 0; i < itemCount; i++){
		for (Mod& m : mods){
			for (string& n : m.items) if (n == items[i].name){
				if (m.mod == "Double Jump") items[i].mods |= 1 << Mods::djump;
				if (m.mod == "High Jump") items[i].mods |= 1 << Mods::hjump;
				if (m.mod == "Speedy") items[i].mods |= 1 << Mods::speedy;
				if (m.mod == "Enhanced Digging") items[i].mods |= 1 << Mods::fastdig;
				if (m.mod == "Fireproof") items[i].mods |= 1 << Mods::fireproof;
				if (m.mod == "Slow-Fall") items[i].mods |= 1 << Mods::slowfall;
				if (m.mod == "XP Buff") items[i].mods |= 1 << Mods::xpbuff;
			}
		}
	}
}

void saveJSON(){
	nlohmann::json js;
	for (int i = 0; i < itemCount; i++){
		item item = items[i];
		nlohmann::json j;
		j["itemID"] = item.itemID;
        j["editableType"] = item.editableType;
        j["itemCategory"] = item.itemCategory;
        j["actionType"] = item.actionType;
        j["hitSoundType"] = item.hitSoundType;
        j["name"] = item.name;
        j["texture"] = item.texture;
        j["textureHash"] = item.textureHash;
        j["itemKind"] = item.itemKind;
        j["val1"] = item.val1;
        j["textureX"] = item.textureX;
        j["textureY"] = item.textureY;
        j["spreadType"] = item.spreadType;
        j["isStripeyWallpaper"] = item.isStripeyWallpaper;
        j["collisionType"] = item.collisionType;
        j["breakHits"] = item.breakHits;
        j["restoreTime"] = item.restoreTime;
        j["clothingType"] = item.clothingType;
        j["rarity"] = item.rarity;
        j["maxAmount"] = item.maxAmount;
        j["extraFile"] = item.extraFile;
        j["extraFileHash"] = item.extraFileHash;
        j["audioVolume"] = item.audioVolume;
        j["petName"] = item.petName;
        j["petPrefix"] = item.petPrefix;
        j["petSuffix"] = item.petSuffix;
        j["petAbility"] = item.petAbility;
        j["seedBase"] = item.seedBase;
        j["seedOverlay"] = item.seedOverlay;
        j["treeBase"] = item.treeBase;
        j["treeLeaves"] = item.treeLeaves;
        j["seedColor"] = item.seedColor;
        j["seedOverlayColor"] = item.seedOverlayColor;
        j["growTime"] = item.growTime;
        j["val2"] = item.val2;
        j["isRayman"] = item.isRayman;
        j["extraOptions"] = item.extraOptions;
        j["texture2"] = item.texture2;
        j["extraOptions2"] = item.extraOptions2;
        j["punchOptions"] = item.punchOptions;
        j["extraFieldUnk_4"] = item.extraFieldUnk_4;
        j["value"] = item.value;
        j["value2"] = item.value2;
        j["unkValueShort1"] = item.unkValueShort1;
        j["unkValueShort2"] = item.unkValueShort2;
		j["newValue"] = item.newValue;
		j["newValue1"] = item.newValue1;
		j["newValue2"] = item.newValue2;
		j["newValue3"] = item.newValue3;
		j["newValue4"] = item.newValue4;
		j["newValue5"] = item.newValue5;
		j["newValue6"] = item.newValue6;
		j["newValue7"] = item.newValue7;
		j["newValue8"] = item.newValue8;
		j["newValue9"] = item.newValue9;
		j["newInt1"] = item.newInt1;
		j["newInt2"] = item.newInt2;
		j["description"] = item.description;
		j["mods"] = item.mods;
        js["items"].push_back(j);
	}
	ofstream o("items.json");
	o << setw(4) << js << endl;
	o.close();
}

int main(){
    decode_itemsDat();
	doneParsing = 0;
    for (int i = 0; i < threadAmount; i++) {
        thread parse(parseWiki, i);
        parse.detach();
    }
    while (doneParsing != threadAmount) std::this_thread::sleep_for(std::chrono::milliseconds(100));
    printf("Done parsing descriptions!\n");
    parseWiki_mods();
	printf("Parsed mods\n");
	saveJSON();
	printf("Saved items.json!\n");
	return 0;
}