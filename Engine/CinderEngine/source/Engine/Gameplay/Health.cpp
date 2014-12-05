/******************************************************************************/
/*!
\file   Health.cpp
\author Anna Pearson
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The players health
*/
/******************************************************************************/
#include "Health.h"
#include "EventSystem.h"
#include "UpdateEvent.h"
#include "CollisionEvent.h"
#include "Core.h"
#include "ObjectSystem.h"
#include "Text.h"

namespace Framework
{
	Health::~Health()
	{
		EVENTSYSTEM->mDisconnect<UpdateEvent, Health>(Events::UPDATEEVENT, this, &Health::Update);
		EVENTSYSTEM->mDisconnect<CollisionEvent, Health>("CollisionEvent", this, &Health::OnCollisionEnter);
	}

	void Health::Serialize(Serializer::DataNode* data)
	{
		Serializer::DataNode* value = data->FindElement(data, "alive");
		value->GetValue(&alive);

		value = data->FindElement(data, "maxRadius");
		value->GetValue(&maxRadius);

		value = data->FindElement(data, "minRadius");
		value->GetValue(&minRadius);

		value = data->FindElement(data, "reFueling");
		value->GetValue(&reFueling);
	}

	void Health::Initialize()
	{
		EVENTSYSTEM->mConnect<UpdateEvent, Health>(Events::UPDATEEVENT, this, &Health::Update);
		EVENTSYSTEM->mConnect<CollisionEvent, Health>("CollisionEvent", this, &Health::OnCollisionEnter);
		gameObject->Health = this;

		currentRadius = maxRadius;
    invincible = false;
	}

	void Health::OnCollisionEnter(CollisionEvent* c)
	{
		float growthRate = .1f;
		currentRadius += growthRate * c->Dt;
	//	gameObject->Transform->Scale(currentRadius / maxRadius);
	}


	void Health::Update(UpdateEvent* e)
	{
    if (!e)
      return;

    if (invincible)
    {
      return;
    }

		float deathRate = 2.0f;
		currentRadius -= deathRate * e->Dt;

    if (currentRadius <= minRadius)
    {
       //OBJECTSYSTEM->ZilchLoadLevel(Zilch::String("WinScreen"));
      //printf("dead");
      GUIText* guiText = reinterpret_cast<GUIText*>(OBJECTSYSTEM->FindObjectByID(4)->GetComponent("GUIText"));
      if (guiText)
      {
        guiText->text = "You ran out of fuel :(";
        //TODO_AUDIO: Play Death Sound/Music
      }
    }
	}

	void Health::reFuel()
	{
		reFueling = true;
	}

  void Health::ToggleInvulnerability()
  {
    invincible = !invincible;
  }

	DefineComponentName(Health);
}