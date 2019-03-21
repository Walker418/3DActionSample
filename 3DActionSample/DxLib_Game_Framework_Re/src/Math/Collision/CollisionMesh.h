#ifndef COLLISION_MESH_H_
#define COLLISION_MESH_H_

#include <string>
#include "../../Math/Vector3.h"
#include "../../Graphic/ModelAsset.h"

// �N���X�F�Փ˔���p���b�V��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class CollisionMesh
{
public:
	// ������
	static void initialize();
	// �I������
	static void finalize();
	// �ǂݍ���
	static bool load(int id, const std::string& file_name, int frame = -1, int div_x = 32, int div_y = 8, int div_z = 32);
	// �폜
	static void erase(int id);
	// ���b�V���̃o�C���h
	static void bind(int id);
	// �`��
	static void draw();
	// �����Ƃ̏Փ˔���
	static bool collide_line(const Vector3& start, const Vector3& end, Vector3* point = nullptr, Vector3* normal = nullptr);
	// ���̂Ƃ̏Փ˔���
	static bool collide_sphere(const Vector3& center, float radius, Vector3* result = nullptr);
	// �J�v�Z���Ƃ̏Փ˔���
	static bool collide_capsule(const Vector3& start, const Vector3& end, float radius, Vector3* result = nullptr);

	// ���b�V���Ɋ܂܂�钸�_�̃��[�J�����W�ł̍ő�l�̎擾
	static Vector3 max_position();
	// ���b�V���Ɋ܂܂�钸�_�̃��[�J�����W�ł̍ŏ��l�̎擾
	static Vector3 min_position();


private:
	// �o�C���h���̃��f��
	static int			model_;
	// ���f���A�Z�b�g
	static ModelAsset	asset_;
};

#endif // !COLLISION_MESH_H_