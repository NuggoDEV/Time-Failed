#include "main.hpp"
#include "Hooks.hpp"

#include "GlobalNamespace/ResultsViewController.hpp"
//#include "GlobalNamespace/"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
using namespace GlobalNamespace;

using namespace UnityEngine;

#include "questui/shared/BeatSaberUI.hpp"
using namespace QuestUI;

std::string SecondsToString(float value)
{
    int minutes = value / 60;
    int seconds = value - minutes * 60;
    std::string minuteStr = std::to_string(minutes);
    std::string secondStr = std::to_string(seconds);

    if (minutes == 0)
    {
        return secondStr + "s";
    }
    else
    {
        return minuteStr + "m :" + secondStr + "s";
    }
}


int endTime;
int timeFailed;
GameObject *container;
TMPro::TextMeshProUGUI *timeFailedText;

MAKE_AUTO_HOOK_MATCH(ResultsViewController_DidActivate, &ResultsViewController::DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (firstActivation)
    {
        container = BeatSaberUI::CreateFloatingScreen({0, 0}, {0, 0.95, 4.1}, {0, 0, 0}, 0.0f, false, false);
        timeFailedText = BeatSaberUI::CreateText(container->get_transform(), "");
        timeFailedText->set_alignment(TMPro::TextAlignmentOptions::Center);
        timeFailedText->set_fontSize(6.0f);
        getLogger().info("Created text");
    }
    container->SetActive(false);

    if (timeFailed != endTime)
    {
        container->set_active(true);
        timeFailedText->set_text("Failed at: " + SecondsToString(timeFailed) + " / " + SecondsToString(endTime));
    }

}

MAKE_AUTO_HOOK_MATCH(ResultsViewController_DidDeactivate, &ResultsViewController::DidDeactivate, void, ResultsViewController* self, bool removedFromHierarchy, bool systemScreenDisabling) {
    ResultsViewController_DidDeactivate(self, removedFromHierarchy, systemScreenDisabling);

    if (timeFailed != endTime)
    {
        container->set_active(false);
    }
}

MAKE_AUTO_HOOK_MATCH(AudioTimeSyncController_Update, &AudioTimeSyncController::Update, void, AudioTimeSyncController* self) {
    AudioTimeSyncController_Update(self);

    endTime = self->get_songEndTime();
    timeFailed = self->songTime;
    getLogger().info("Updated time");
}