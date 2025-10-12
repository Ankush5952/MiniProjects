#include "preset_manager.h"

std::string shape2string(ParticleSystem::ParticleShape shape)
{
    std::string s = "";
    switch (shape)
    {
        case ParticleSystem::CIRCLE:
            s = "CIRCLE";
            break;
        case ParticleSystem::TRIANGLE:
            s = "TRIANGLE";
            break;
        case ParticleSystem::SQUARE:
            s = "SQUARE";
            break;
        default:
            s = "CIRCLE";
    }
    return s;
}

ParticleSystem::ParticleShape string2shape(std::string str)
{
    if (str == "CIRCLE") return ParticleSystem::CIRCLE;
    if (str == "TRIANGLE") return ParticleSystem::TRIANGLE;
    if (str == "SQUARE") return ParticleSystem::SQUARE;

    return ParticleSystem::CIRCLE;
}

std::string collission2string(ParticleSystem::CollissionAlgo a)
{
    switch (a)
    {
    case ParticleSystem::BOUNCE:
        return "BOUNCE";
        break;
    case ParticleSystem::ABSORB:
        return "ABSORB";
        break;
    case ParticleSystem::CONVERT:
        return "CONVERT";
        break;
    case ParticleSystem::DESTROY:
        return "DESTROY";
        break;
    case ParticleSystem::FLOW:
        return "FLOW";
        break;
    case ParticleSystem::REPEL:
        return "REPEL";
        break;
    case ParticleSystem::STICK:
        return "STICK";
        break;
    default:
        return "BOUNCE";
    }
}

ParticleSystem::CollissionAlgo string2collission(std::string str)
{
    if (str == "DESTROY") return ParticleSystem::DESTROY;
    if (str == "FLOW") return ParticleSystem::FLOW;
    if (str == "BOUNCE") return ParticleSystem::BOUNCE;
    if (str == "CONVERT")return ParticleSystem::CONVERT;
    if (str == "ABSORB") return ParticleSystem::ABSORB;
    if (str == "STICK") return ParticleSystem::STICK;
    if (str == "REPEL") return ParticleSystem::REPEL;
    
    return ParticleSystem::BOUNCE;
}

json ParticleSystem::Preset::toJson() const
{
    json j;

    j["name"] = name;
    j["shape"] = shape2string(shape);
    j["size"] = size;
    j["lifetime"] = lifetime;
    j["color"] = {
        {"r",color.r},
        {"g",color.g},
        {"b",color.b},
        {"a",color.a}
    };
    j["CollissionResponse"] = collission2string(algo);
    j["velocity"] = {
        {"x", velocity.x},
        {"y", velocity.y}
    };

    return j;
}

ParticleSystem::Preset ParticleSystem::Preset::fromJson(const json& j)
{
    Preset preset;
    preset.name = j.value("name", "Unnamed");
    preset.shape = string2shape(j.value("shape", "CIRCLE"));
    preset.size = j.value("size", 1);
    preset.lifetime = j.value("lifetime", 0.5f);
    
    if (j.contains("color"))
    {
        preset.color.r = j["color"].value("r", 255);
        preset.color.g = j["color"].value("g", 255);
        preset.color.b = j["color"].value("b", 255);
        preset.color.a = j["color"].value("a", 255);
    }

    preset.algo = string2collission(j.value("CollissionResponse", "BOUNCE"));

    if (j.contains("velocity"))
    {
        preset.velocity.x = j["velocity"].value("x", 0);
        preset.velocity.y = j["velocity"].value("y", 0);
    }

    return preset;
}

const ParticleSystem::Preset& ParticleSystem::PresetManager::getCurrentPreset() const
{
    if (presets.empty())
    {
        static Preset defaultPreset;
        return defaultPreset;
    }
    return presets[currentPresetIndex];
}

const ParticleSystem::Preset& ParticleSystem::PresetManager::getPreset(int index) const
{
    if (index < 0 || index >= presets.size())throw std::exception("Invalid preset index to fetch");

    return presets[index];
}

int ParticleSystem::PresetManager::getCurrentPresetIndex() const
{
    return currentPresetIndex;
}

int ParticleSystem::PresetManager::getPresetCount() const
{
    return presets.size();
}

std::vector<std::string> ParticleSystem::PresetManager::getPresetNames() const
{
    std::vector<std::string> presetNames = {};

    for (const auto& i : presets)
    {
        presetNames.push_back(i.name);
    }

    return presetNames;
}

void ParticleSystem::PresetManager::addPreset(const Preset& p)
{
    presets.push_back(p);
}

void ParticleSystem::PresetManager::removePreset(int index)
{
    if (index < 0 || index >= presets.size()) throw std::exception("Invalid index to delete preset");

    presets.erase(presets.begin() + index);

    if (currentPresetIndex >= index) currentPresetIndex = std::max(0, std::min(currentPresetIndex - 1, (int)presets.size()));
}

void ParticleSystem::PresetManager::nextPreset()
{
    currentPresetIndex = std::min(currentPresetIndex + 1,(int) presets.size()-1);
}

void ParticleSystem::PresetManager::prevPreset()
{
    currentPresetIndex = std::max(currentPresetIndex - 1, 0);
}

void ParticleSystem::PresetManager::setPresetIndex(int index)
{
    currentPresetIndex = std::max(0, std::min(index, (int)presets.size()-1));
}

bool ParticleSystem::PresetManager::loadFromFile()
{
    std::ifstream file(filePath);
    
    if (!file.is_open()) return false;

    try
    {
        json j;
        file >> j;
        file.close();

        if (j.contains("presets") && j["presets"].is_array())
        {
            for (const auto& pj : j["presets"])
            {
                presets.push_back(Preset::fromJson(pj));
            }
        }

        if (presets.empty())
        {
            return false;
        }

        currentPresetIndex = 0;
        return true;
    }
    catch (...)
    {
        file.close();
        return false;
    }
}

bool ParticleSystem::PresetManager::saveToFile()
{
    try
    {
        json j;
        j["version"] = "1.0";
        j["presets"] = json::array();

        for (const auto& p : presets)
        {
            j["presets"].push_back(p.toJson());
        }

        std::ofstream file(filePath);

        if (!file.is_open()) return false;

        file << j.dump(4);
        file.close();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

void ParticleSystem::PresetManager::createDefaultPresets()
{
    presets.clear();

    //Fire
    presets.push_back(Preset(
        "Fire",
        CIRCLE,
        5,
        2.0f,
        ORANGE,
        FLOW,
        { 50.0f, 100.0f }
    ));

    //Smoke
    presets.push_back(Preset(
        "Smoke",
        CIRCLE,
        8,
        4.0f,
        GRAY,
        FLOW,
        { 30.0f,60.0f }
    ));

    //Bouncy Balls
    presets.push_back(Preset(
        "Bouncy",
        CIRCLE,
        10,
        -1.0f,
        RED,
        BOUNCE,
        { 100.0f, 100.0f }
    ));

    //Water
    presets.push_back(Preset(
        "Water",
        CIRCLE,
        2,
        -1.0f,
        DARKBLUE,
        FLOW,
        { 50.0f, 50.0f }
    ));

    //Sticky
    presets.push_back(Preset(
        "Sticky",
        SQUARE,
        3,
        3.0f,
        PINK,
        STICK,
        { 80.0f, 80.0f }
    ));

    //Explosive
    presets.push_back(Preset(
        "Explosive",
        CIRCLE,
        4,
        1.5f,
        YELLOW,
        REPEL,
        { 150.0f, 150.0f }
    ));

    //Converter
    presets.push_back(Preset(
        "Converter",
        TRIANGLE,
        3,
        2.0f,
        GREEN,
        CONVERT,
        { 80.0f, 70.0f }
    ));

    //Absorber
    presets.push_back(Preset(
        "Absorber",
        TRIANGLE,
        8,
        2.0f,
        BROWN,
        ABSORB,
        { 50.0f, 50.0f }
    ));

    //Destroyer
    presets.push_back(Preset(
        "Destroyer",
        SQUARE,
        12,
        5.0f,
        PURPLE,
        DESTROY,
        { 200.0f, 200.0f }
    ));

    currentPresetIndex = 0;
}

ParticleSystem::PresetManager::PresetManager(const std::string& pathToFile)
{
    filePath = pathToFile;
    currentPresetIndex = 0;

    if (!loadFromFile())
    {
        createDefaultPresets();
        saveToFile();
    }
}
