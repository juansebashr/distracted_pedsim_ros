/**
 * Copyright 2014 Social Robotics Lab, University of Freiburg
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    # Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    # Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *    # Neither the name of the University of Freiburg nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \author Billy Okal <okal@cs.uni-freiburg.de>
 * \author Sven Wehner <mail@svenwehner.de>
 */

#ifndef _agentstatemachine_h_
#define _agentstatemachine_h_

#include <ros/ros.h>
#include <QObject>

// Forward Declarations
class Agent;
class AttractionArea;
class IndividualWaypointPlanner;
class QueueingWaypointPlanner;
class GroupWaypointPlanner;
class ShoppingPlanner;

class AgentStateMachine : public QObject
{
    Q_OBJECT

    // Enums
    // TODO - switch to enum classes
   public:
    typedef enum
    {
        StateNone,
        StateWaiting,
        StateQueueing,
        StateWalking,
        StateGroupWalking,
        StateShopping,
        StateTalking,
        StateWorking,
        StateLiftingForks,
        StateLoading,
        StateLoweringForks,
        StateDriving,
        StateTellStory,
        StateGroupTalking,
        StateListening,
        StateTalkingAndWalking,
        StateListeningAndWalking,
        StateReachedShelf,
        StateRunning,
        StateBackUp,
        StateRequestingService,
        StateReceivingService,
        StateDrivingToInteraction,
        StateProvidingService,
        StateRequestingGuide,
        StateFollowingGuide,
        StateRequestingFollower,
        StateGuideToGoal,
        StateClearingGoal,
        StateWaitForTrigger,
        StateWaitForTimer,
    } AgentState;

    typedef enum
    {
        DistractionNone,
        DistractionKremer,
        DistractionPam,
        DistractionPamModified,
    } AgentDistraction;

    // Constructor and Destructor
   public:
    AgentStateMachine(Agent* agentIn);
    virtual ~AgentStateMachine();

    // Signals
   signals:
    void stateChanged(AgentState newState);

   public slots:
    void loseAttraction();

   public:
    void doStateTransition();
    AgentState getCurrentState();
    AgentDistraction getCurrentDistraction();
    void activateState(AgentState stateIn);
    void activateDistraction(AgentDistraction distractionIn);
    static QString stateToName(AgentState stateIn);
    static std::string distractionToName(AgentDistraction distractionIn);

   protected:
    void deactivateState(AgentState stateIn);
    void deactivateDistraction();
    bool checkGroupForAttractions(AttractionArea** attractionOut = nullptr) const;
    double getRandomDuration(double baseTime);

    Agent* agent;

    // → State Machine
    AgentState state;
    AgentState normalState;

    // → Distraction State Machine
    AgentDistraction distractionState;

    // → Waypoint Planner
    IndividualWaypointPlanner* individualPlanner;
    QueueingWaypointPlanner* queueingPlanner;
    GroupWaypointPlanner* groupWaypointPlanner;
    ShoppingPlanner* shoppingPlanner;

    // → Attraction
    AttractionArea* groupAttraction;
    bool shallLoseAttraction;

    ros::WallTime startTimestamp;
    double stateMaxDuration;  // in seconds

    int currentEpisode;
};

#endif
