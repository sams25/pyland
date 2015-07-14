#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index_container.hpp>
#include <glm/vec2.hpp>
#include <map>
#include <string>
#include <vector>

#include "animation_frames.hpp"
#include "map.hpp"
#include "object.hpp"
#include "walkability.hpp"

#ifndef KEYHASH
#define KEYHASH
struct KeyHash {
    std::size_t operator()(const glm::vec2 &key) const {
        return boost::hash<float>()(key.x * 2048.0f + key.y);
    }
};
#endif

#ifndef INSERTION_ORDER
#define INSERTION_ORDER
struct insertion_order {};
#endif

template <typename T>
using OrderedHashSet = boost::multi_index_container<
    T,
    boost::multi_index::indexed_by<
        // hashed (like std::unordered_map)
        boost::multi_index::hashed_unique<boost::multi_index::identity<T>, KeyHash>,
        // ordered by insertion (like std::list)
        boost::multi_index::sequenced<boost::multi_index::tag<insertion_order>>
    >
>;

///
/// Represents an object which can be rendered on the map.
/// The only CPP class needed for this. All other functinality is written
//  in python.
///
class MapObject : public Object {
protected:

    ///
    /// The focus icon, to move with sprite and hide, depending on if mapobject is in focus;
    ///
    bool is_focus;

    ///
    /// The focus icon, to move with sprite and hide, depending on if mapobject is in focus;
    ///
    int focus_icon_id;

    ///
    /// Render the object above sprites as an overlay
    ///
    bool render_above_sprite;

    ///
    /// Whether you can step all over it
    ///
    Walkability walkability;

    ///
    /// Re-create the blockers for blocking one's path
    ///
    void regenerate_blockers();

    ///
    /// The blockers for blocking one's path
    ///
    std::vector<Map::Blocker> body_blockers;

    ///
    /// The position of the object
    ///
    glm::vec2 position;

    ///
    /// An ordered container of positions that the map object has been on,
    /// as recorded by set_state_on_moving_finish().
    ///
    OrderedHashSet<glm::vec2> positions;

    ///
    /// Tiles that the object is blocking, probably
    /// by standing on.
    ///
    std::map<std::string, Map::Blocker> blocked_tiles;

    ///
    /// The object's moving state
    ///
    bool moving = false;

    ///
    /// The challenge that created or now owns the object.
    ///
    /// This must be set manually by the challenge,
    /// and may be null at any time.
    ///
    Challenge *challenge = nullptr;

public:
    ///
    /// Constructs a map object
    /// @param position
    ///     the (x, y) position of the sprite on the map
    /// @param name
    ///     the name of the sprite
    /// @param walkability
    ///     the walkability properties of the sprite
    /// @param tile
    ///     the sprite's image, referenced by an id:sheet_name pair
    /// @param walk_frames
    ///     walking frames to animate movement.
    ///
    MapObject(glm::vec2 position,
              std::string name,
              Walkability walkability,
              AnimationFrames walk_frames);


    virtual ~MapObject();

    glm::vec2 get_position() { return position; }

    virtual void set_position(glm::vec2 position);

    ///
    /// manage collisions for spirtes as they move
    /// @param target
    ///     tile the sprite it moving to
    ///
    virtual void set_state_on_moving_start(glm::ivec2 target);

    ///
    /// manage collisions for sprites as they move
    ///
    virtual void set_state_on_moving_finish();

    ///
    /// Determine if the object should be rendered above sprites
    /// @return if the object should render above sprites
    ///
    virtual bool render_above_sprites() { return render_above_sprite; }

    ///
    /// If the object is to be rendered above sprites
    /// @param _render_above_sprites true if the object should be above sprites
    ///
    virtual void set_render_above_sprites(bool _render_above_sprites) { render_above_sprite = _render_above_sprites; }

    ///
    /// Generate the texture coordinate data for the object
    ///
    virtual void generate_tex_data(std::pair<int, std::string> tile);

    ///
    /// Change the tile of the sprite to that of the given name
    ///
    virtual void set_tile(std::pair<int, std::string> tile);

    ///
    /// Generate the vertex data for the object
    ///
    virtual void generate_vertex_data();

    ///
    /// Load the textures that are being used by the object
    ///
    virtual void load_textures(std::pair<int, std::string> tile);

    ///
    /// Initialise the shaders that are being used by the object
    ///
    virtual bool init_shaders();

    ///
    /// Set the object's moving status
    /// @param _moving if the object is moving
    ///
    virtual void set_moving(bool moving) { this->moving = moving; }

    ///
    /// Set whether the object creates blockers
    /// to prevent sprites moving onto the squares
    /// occupied by the sprite.
    ///
    /// @param walkability
    ///     The value to set the internal walkability to.
    ///
    virtual void set_walkability(Walkability walkability);

    ///
    /// Get if the object is moving
    /// @return the object's moving status
    ///
    virtual bool is_moving() { return moving; }

    ///
    /// Walking frames to animate movement.
    ///created
    AnimationFrames frames;

    ///
    /// An ordered container of positions that the map object has been on,
    /// as recorded by set_state_on_moving_finish().
    ///
    OrderedHashSet<glm::vec2> const &get_positions();

    ///
    /// Set the challenge that created or now owns the object.
    ///
    /// It is permitted to call this with nullptr at any time.
    ///
    /// @warning
    ///     An object should not outlive its challenge pointer.
    ///
    void set_challenge(Challenge *challenge);

    ///
    /// The challenge that created or now owns the object.
    ///
    /// This is set manually by the challenge,
    /// and may be null at any time.
    ///
    Challenge const *get_challenge();

    void set_focus(bool _is_focus); //TODO BLEH COMMENT THIS AFTER WORKING OUT WHAT IT DOES 
};

#endif
