module;
#include <ncurses.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

export module audio.sound;

import <memory>;
import <string>;
import <unordered_map>;

export namespace age {

// Abstract class for audio management
class SoundSystem {
public:
    virtual ~SoundSystem() = default;

    virtual void play(const std::string& soundId) = 0;
    virtual void stopAll() = 0;

    virtual void setMuted(bool muted) = 0;
    virtual bool isMuted() const = 0;
    void toggleMute();
};

// SoundClip (RAII wrapper for Mix_Chunk*)
class SoundClip {
public:
    explicit SoundClip(const std::string& path);
    
    ~SoundClip() = default;
    
    // Non-copyable
    SoundClip(const SoundClip&) = delete;
    SoundClip& operator=(const SoundClip&) = delete;
    
    // Movable
    SoundClip(SoundClip&& other) noexcept;
    SoundClip& operator=(SoundClip&& other) noexcept;
    
    bool isValid() const;
    
    void play(int channel = -1, int loops = 0);

private:
    Mix_Chunk* chunk_;
};

// SDL Implementation (real audio using SDL2_mixer)
class SDLSoundSystem final : public SoundSystem {
public:
    SDLSoundSystem();
    
    ~SDLSoundSystem() override;
    
    // Load a sound file
    void loadSound(const std::string& id, const std::string& path);
    
    void play(const std::string& soundId) override;
    
    void stopAll() override;
    
    void setMuted(bool muted) override;
    bool isMuted() const override;
    
    bool isInitialized() const;

private:
    bool initialized_;
    bool muted_;
    std::unordered_map<std::string, std::unique_ptr<SoundClip>> sounds_;
};

// Terminal implementation (uses ncurses beep() for terminal audio)
class TerminalSoundSystem final : public SoundSystem {
public:
    TerminalSoundSystem() = default;
    ~TerminalSoundSystem() override = default;

    void play(const std::string& soundId) override;

    void stopAll() override;

    void setMuted(bool muted) override;
    bool isMuted() const override;

private:
    bool muted_;
};

// Silent implementation (for testing or disabled audio)
class NullSoundSystem final : public SoundSystem {
public:
    NullSoundSystem() = default;
    ~NullSoundSystem() override = default;

    void play(const std::string&) override;
    void stopAll() override;

    void setMuted(bool muted) override;
    bool isMuted() const override;

private:
    bool muted_;
};

// Factory functions
enum class SoundBackend {
    Terminal,  // ncurses beep/flash
    SDL,       // SDL2_mixer (real audio files)
    Null       // Silent
};

inline std::unique_ptr<SoundSystem> createSoundSystem(SoundBackend backend = SoundBackend::Terminal) {
    switch (backend) {
        case SoundBackend::SDL:
            return std::make_unique<SDLSoundSystem>();
        case SoundBackend::Null:
            return std::make_unique<NullSoundSystem>();
        case SoundBackend::Terminal:
        default:
            return std::make_unique<TerminalSoundSystem>();
    }
}

}
