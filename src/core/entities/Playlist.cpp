#include "core/entities/Playlist.hpp"

#include "core/entities/Song.hpp"
#include "core/entities/User.hpp"


namespace core {

	Playlist::Playlist() : Entity(0), _titulo(""), _user_id(0), _user(nullptr), _songs(), _loader() {}

	Playlist::Playlist(const unsigned id, const std::string titulo)
		: Entity(id), _titulo(titulo), _user_id(0), _user(nullptr), _songs(), _loader() {}

	Playlist::~Playlist() = default;

	std::string Playlist::getTitulo() { return _titulo; }

	std::string Playlist::getTitle() const { return _titulo; }

	void Playlist::setTitulo(const std::string nome) { _titulo = nome; }

	bool Playlist::operator==(const Entity& other) const {
		const Playlist* otherPl = dynamic_cast<const Playlist*>(&other);
		if(otherPl == nullptr) return false;
		return (otherPl->getId() == this->getId());
	}

	bool Playlist::operator!=(const Entity& other) const {
		return !(*this == other);
	}

	std::vector<std::shared_ptr<Song>> Playlist::getSongs() const { return _songs; }

	void Playlist::setSongsLoader(
		const std::function<std::vector<std::shared_ptr<Song>>()>& loader) {
		if (loader) {
			_loader = loader;
		}
	}

	void Playlist::addSong(Song& song) {
		_songs.push_back(std::make_shared<Song>(song));
	}

	bool Playlist::switchSong(unsigned id, unsigned index) {
			try {
				if (index > _songs.size()) throw std::out_of_range("index maior que o tamanho da playlist");

				auto it = _songs.begin();
				while (it != _songs.end()) {
					if ((*it)->getId() == id) {
						break;
					}
					++it;
				}

				if (it == _songs.end()) return false;
				auto node = *it;
				_songs.erase(it);
				if (index >= _songs.size()) {
					_songs.push_back(node);
				} else {
					_songs.insert(_songs.begin() + index, node);
				}
				return true;
			} catch (const std::out_of_range &e) {
				std::cerr << "switchSong: índice inválido: " << e.what() << std::endl;
				return false;
			} catch (const std::exception &e) {
				std::cerr << "switchSong: erro inesperado: " << e.what() << std::endl;
				return false;
			}
	}

	bool Playlist::removeSong(unsigned id) {
		std::vector<std::shared_ptr<Song>>::iterator it = _songs.begin();
        while(it != _songs.end()) {
            if(it->get()->getId() == id) {
                break;
            }
        }
		if (it == _songs.end()) return false;
		_songs.erase(it);
		return true;
	}

	std::shared_ptr<Song> Playlist::findSongById(unsigned songId) {
		std::vector<std::shared_ptr<Song>>::iterator it = _songs.begin();
        while(it != _songs.end()) {
            if(it->get()->getId() == songId) {
                break;
            }
        }
		if (it == _songs.end()) return nullptr;
		return std::static_pointer_cast<Song>(*it);
	}

	std::shared_ptr<Song> Playlist::findSongByTitle(const std::string& title) {
		std::vector<std::shared_ptr<Song>>::iterator it = _songs.begin();
        while(it != _songs.end()) {
            if(it->get()->getTitle() == title) {
                break;
            }
        }
		if (it == _songs.end()) return nullptr;
		return std::static_pointer_cast<Song>(*it);
	}

	int Playlist::calculateTotalDuration() {
		int total = 0;
		for (const auto& s : _songs) {
			if (!s) continue;
			int d = s->getDuration();
			if (d > 0) total += d;
		}
		return total;
	}

	std::string Playlist::getFormattedDuration() {
		int totalSeconds = calculateTotalDuration();
		int h = totalSeconds / 3600;
		int m = (totalSeconds % 3600) / 60;
		int s = totalSeconds % 60;

		std::ostringstream ss;
		if (h > 0) {
			ss << h << ":" << (m < 10 ? "0" : "") << m << ":" << (s < 10 ? "0" : "") << s;
		} else {
			ss << m << ":" << (s < 10 ? "0" : "") << s;
		}
		return ss.str();
	}

	std::shared_ptr<Song> Playlist::getNextSong(Song &current) {
		unsigned curId = current.getId();

		for (size_t i = 0; i < _songs.size(); ++i) {
			if (!_songs[i]) continue;
			if ((_songs[i]->getId() == curId) || (curId == 0)) {
				if (i + 1 < _songs.size()) return std::static_pointer_cast<Song>(_songs[i + 1]);
				return nullptr;
			}
		}
		return nullptr;
	}

	std::shared_ptr<Song> Playlist::getPreviousSong(Song &current) {
		unsigned curId = current.getId();

		for (size_t i = 0; i < _songs.size(); ++i) {
			if (!_songs[i]) continue;
			if ((_songs[i]->getId() == curId) || (curId == 0)) {
				if (i == 0) return nullptr;
				return std::static_pointer_cast<Song>(_songs[i - 1]);
			}
		}
		return nullptr;
	}

	std::shared_ptr<Song> Playlist::getSongAt(int index) {
			if (index < 0) return nullptr;
			try {
				return _songs.at(static_cast<size_t>(index));
			} catch (const std::out_of_range &e) {
				std::cerr << "getSongAt: índice fora do intervalo: " << e.what() << std::endl;
				return nullptr;
			}
	}

	std::vector<std::shared_ptr<IPlayableObject>> Playlist::getPlayableObjects() const {
		std::vector<std::shared_ptr<IPlayableObject>> out;
		for (const auto& s : _songs) {
			if (!s) continue;
			auto objs = s->getPlayableObjects();
			out.insert(out.end(), objs.begin(), objs.end());
		}
		return out;
	}

	std::shared_ptr<User> Playlist::getUser() const { return _user; }

	void Playlist::setUser(const User& user) {
		_user = std::make_shared<User>(user);
		_user_id = user.getId();
	}

}
