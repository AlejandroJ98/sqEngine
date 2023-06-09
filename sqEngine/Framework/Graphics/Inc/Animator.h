#pragma once
namespace sqEngine::Graphics
{
	//struct Bone;
	class Model;
	/*struct IAnimator
	{
		virtual Math::Matrix4 GetTransform(const Bone* bone) const = 0;
	};*/

	class Animator final //: public IAnimator
	{
	public:
		void Initialize(const Model* model);

		void PlayAnimation(size_t clipIndex);//bool looping=true
		void Update(float deltaTime);
		bool IsFinished() const;

		/*Math::Vector3 Getposition(const Bone* bone) const;
		Math::Quaternion Getrotation(const Bone* bone) const;
		Math::Vector3 Getscale(const Bone* bone) const;


		Math::Matrix4 GetTransform(const Bone* bone) const override;*/
	private:
		const Model* mModel = nullptr;
		size_t mClipIndex = 0;
		float mAniamtionTick = 0;
		//bool mLooping=false;
	};

	/*class AnimationBlender :public IAnimator
	{

	public:
		Animator* AddAnimator();
		void SetBlendWeight(size_t index, float weight);
		void Update(float deltaTime);
		Math::Matrix4 GetTransform(const Bone* bone) const override;
	private:
		struct Entry
		{

			Animator animator;
			float weight = 1.0f;

		};
		std::vector<std::unique_ptr<Entry>> mEntries;

	};*/

}