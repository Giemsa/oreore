#include "TutorialPhase.h"
#include "TutorialBase.h"

namespace oreore
{
    namespace Tutorial
    {
        bool TutorialPhase::proceed(const size_t index)
        {
            switch(type)
            {
                case PhaseType::Begin:
                    return base->showTutorial([this, index]() {
                        played = true;
                        return proc(index);
                    });

                case PhaseType::Stay:
                    played = true;
                    return proc(index);

                case PhaseType::End:
                    base->hideTutorial();
                    played = true;
                    return proc(index);
            }
        }

        bool TutorialPhase::loadPhase(const picojson::value &data)
        {
            {
                const picojson::value &v = data.get("trigger");
                if(!v.is<double>())
                {
                    return false;
                }

                trigger = static_cast<int>(v.get<double>());
            }

            {
                played = data.get("played").evaluate_as_boolean();
            }

            return true;
        }

        bool TutorialPhase::savePhase(picojson::array &list) const
        {
            picojson::object obj;

            obj.insert(std::make_pair("trigger", picojson::value(static_cast<double>(trigger))));
            obj.insert(std::make_pair("played", picojson::value(played)));

            list.push_back(picojson::value(obj));
            return true;
        }
    }
}
