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
TMPro::TextMeshProUGUI *timeFailedText;

MAKE_AUTO_HOOK_MATCH(ResultsViewController_DidActivate, &ResultsViewController::DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if (firstActivation && timeFailed < endTime)
    {
        timeFailedText = BeatSaberUI::CreateText(self->get_transform(), "", false, {0.1, -12.5});
        timeFailedText->set_alignment(TMPro::TextAlignmentOptions::Center);
        timeFailedText->set_fontSize(6.0f);
        getLogger().info("Created text");
    }

    if (timeFailed < endTime)
    {
        timeFailedText->set_text("Failed at: " + SecondsToString(timeFailed) + " / " + SecondsToString(endTime));
    }
    else if (timeFailed == endTime)
    {
        timeFailedText->set_text("");
    }

}

MAKE_AUTO_HOOK_MATCH(AudioTimeSyncController_Update, &AudioTimeSyncController::Update, void, AudioTimeSyncController* self) {
    AudioTimeSyncController_Update(self);

    endTime = self->get_songEndTime();
    timeFailed = self->songTime;
    getLogger().info("Updated time");
}