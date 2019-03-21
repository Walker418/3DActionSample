#include "Billboard.h"

// �N���X�F�r���{�[�h
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �o�C���h���̃C���[�W
int Billboard::image_{ -1 };
// �e�N�X�`���A�Z�b�g
TextureAsset Billboard::asset_;

// ������
void Billboard::initialize()
{
	// �I�������Ɠ���
	finalize();
}

// �I������
void Billboard::finalize()
{
	asset_.clear();
	image_ = -1;
}

// �ǂݍ���
bool Billboard::load(int id, const std::string& file_name)
{
	return asset_.load(id, file_name);
}

// �폜
void Billboard::erase(int id)
{
	image_ = (image_ == asset_[id]) ? -1 : image_;
	asset_.erase(id);
}

// �o�C���h
void Billboard::bind(int id)
{
	image_ = asset_[id];
}

// �`��
void Billboard::draw(const VECTOR& position, float size, float cx, float cy, float angle)
{
	DrawBillboard3D(position, cx, cy, size, angle, image_, TRUE);
}