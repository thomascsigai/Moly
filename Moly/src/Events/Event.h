#pragma once

#include <Core.h>
#include <functional>
#include <memory>
#include <string>

namespace Moly
{
	// Classe de base pour tous les événements
	class MOLY_API Event {
	public:
		virtual ~Event() = default;
		virtual std::string GetEventType() const = 0;
	};

	class MOLY_API EventManager {
	public:
		using EventCallback = std::function<void(const std::shared_ptr<Event>&)>;

		// S'abonner à un type d'événement spécifique
		void Subscribe(const std::string& eventType, EventCallback callback) {
			subscribers[eventType].push_back(callback);
		}

		// Publier un événement à tous les abonnés
		void Publish(const std::shared_ptr<Event>& event) const {
			auto it = subscribers.find(event->GetEventType());
			if (it != subscribers.end()) {
				for (const auto& callback : it->second) {
					callback(event);
				}
			}
		}

	private:
		std::unordered_map<std::string, std::vector<EventCallback>> subscribers;
	};
}