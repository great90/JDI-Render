#ifndef SPRITE_FRAME
#define SPRITE_FRAME

#include "core/resource.h"
#include "resources/texture.h"

class SpriteFrames : public Resource {

	GDCLASS(SpriteFrames, Resource);

	struct Anim {

		float speed;
		bool loop;
		Vector<Ref<Texture> > frames;

		Anim() {
			loop = true;
			speed = 5;
		}

		StringName normal_name;
	};

	Map<StringName, Anim> animations;

	Array _get_frames() const;
	void _set_frames(const Array &p_frames);

	Array _get_animations() const;
	void _set_animations(const Array &p_animations);

	Vector<String> _get_animation_list() const;

protected:
	static void _bind_methods();

public:
	void add_animation(const StringName &p_anim);
	bool has_animation(const StringName &p_anim) const;
	void remove_animation(const StringName &p_anim);
	void rename_animation(const StringName &p_prev, const StringName &p_next);

	void get_animation_list(List<StringName> *r_animations) const;
	Vector<String> get_animation_names() const;

	void set_animation_speed(const StringName &p_anim, float p_fps);
	float get_animation_speed(const StringName &p_anim) const;

	void set_animation_loop(const StringName &p_anim, bool p_loop);
	bool get_animation_loop(const StringName &p_anim) const;

	void add_frame(const StringName &p_anim, const Ref<Texture> &p_frame, int p_at_pos = -1);
	int get_frame_count(const StringName &p_anim) const;
	_FORCE_INLINE_ Ref<Texture> get_frame(const StringName &p_anim, int p_idx) const {

		const Map<StringName, Anim>::Element *E = animations.find(p_anim);
		ERR_FAIL_COND_V_MSG(!E, Ref<Texture>(), "Animation '" + String(p_anim) + "' doesn't exist.");
		ERR_FAIL_COND_V(p_idx < 0, Ref<Texture>());
		if (p_idx >= E->get().frames.size())
			return Ref<Texture>();

		return E->get().frames[p_idx];
	}

	_FORCE_INLINE_ Ref<Texture> get_normal_frame(const StringName &p_anim, int p_idx) const {

		const Map<StringName, Anim>::Element *E = animations.find(p_anim);
		ERR_FAIL_COND_V_MSG(!E, Ref<Texture>(), "Animation '" + String(p_anim) + "' doesn't exist.");
		ERR_FAIL_COND_V(p_idx < 0, Ref<Texture>());

		const Map<StringName, Anim>::Element *EN = animations.find(E->get().normal_name);

		if (!EN || p_idx >= EN->get().frames.size())
			return Ref<Texture>();

		return EN->get().frames[p_idx];
	}

	void set_frame(const StringName &p_anim, int p_idx, const Ref<Texture> &p_frame) {
		Map<StringName, Anim>::Element *E = animations.find(p_anim);
		ERR_FAIL_COND_MSG(!E, "Animation '" + String(p_anim) + "' doesn't exist.");
		ERR_FAIL_COND(p_idx < 0);
		if (p_idx >= E->get().frames.size())
			return;
		E->get().frames.write[p_idx] = p_frame;
	}
	void remove_frame(const StringName &p_anim, int p_idx);
	void clear(const StringName &p_anim);
	void clear_all();

	SpriteFrames();
};

#endif // SPRITE_FRAME
