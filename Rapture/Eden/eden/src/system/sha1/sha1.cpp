#include <eden/include/system/sha1/sha1.h>

#include <eden/include/math/math_utility.h>


using namespace std;

EDENS_NAMESPACE_BEGIN



// �g�p�萔�̒�`
constexpr u8 BitPerByte = 8; // Bit
constexpr u8 MessageBlockSize = 64; // Byte
constexpr u8 MessageLengthSize = 8; // Byte
constexpr u8 TempWordNum = 80;
constexpr u8 FirstWord = 16;
constexpr u8 WordSize = 4; // Byte
constexpr u8 DigestSize = 20; // Byte



// �n�b�V���v�Z�Ŏg�p����萔�l
const array<u32, CSHA1::SequenceNum> CSHA1::m_HashConstant =
{
	0x5A827999,    /*  0�`19��ڂ̉��Z�Ŏg�p */
	0x6ED9EBA1,    /* 20�`39��ڂ̉��Z�Ŏg�p */
	0x8F1BBCDC,    /* 40�`59��ڂ̉��Z�Ŏg�p */
	0xCA62C1D6,	   /* 60�`79��ڂ̉��Z�Ŏg�p */
};

// �V�[�P���X���̃`�F�b�N�p�֐�
const array<CSHA1::SequenceCheck, CSHA1::SequenceNum> CSHA1::m_CheckFunction =
{
	CSHA1::Check00,
	CSHA1::Check20,
	CSHA1::Check40,
	CSHA1::Check60,
};

// �n�b�V���v�Z�Ŏg�p����_���֐�
const array<CSHA1::Logic, CSHA1::SequenceNum> CSHA1::m_LogicFunction =
{
	CSHA1::Logic00,
	CSHA1::Logic20,
	CSHA1::Logic40,
	CSHA1::Logic60,
};

// -------

// �������Ŏg�p����ϐ��Q
struct FSHAProxy
{
	// ���b�Z�[�W�u���b�N(64Byte�Œ�)
	array<u8, MessageBlockSize> BlockArray = {};

	// �ꎞ�n�b�V��
	array<u32,5> TempHash = 
	{
		0x67452301,
		0xEFCDAB89,
		0x98BADCFE,
		0x10325476,
		0xC3D2E1F0,
	};

	// ���b�Z�[�W�T�C�Y
	u64 MessageSize = 0;

	// ��������Byte��
	u64 ProcessByteNum = 0;
};


CSHA1::CSHA1(const char* _string)
{
	string message(_string);
	SHA1Main(message);
}

CSHA1::CSHA1(const string& _string)
{
	SHA1Main(_string);
}

void CSHA1::SHA1Main(const string& _string)
{
	FSHAProxy proxy = {};
	proxy.MessageSize = _string.size();
	if (proxy.MessageSize <= 0) { return; }

	auto BlockIndex = [&]()
	{
		return proxy.ProcessByteNum % MessageBlockSize;
	};

	// �ꎞ�ϐ������Ė��O���k�߂�
	auto& blockArray = proxy.BlockArray;
	auto& processByteNum = proxy.ProcessByteNum;

	// ���b�Z�[�W�u���b�N�P�ʂɃ��b�Z�[�W����؂��Ă���
	for (auto& elem : _string)
	{
		blockArray[BlockIndex()] = elem;
		++processByteNum;

		if (BlockIndex() == 0)
		{
			SHA1Process(&proxy);
		}
	}

	// �܂���0x80�𖄂߂�
	blockArray[BlockIndex()] = 0x80;
	++processByteNum;


	u8 realBlockSize = MessageBlockSize - MessageLengthSize;

	// ������������64�Ŋ������]�肪�A56��葽���̂Ƃ�
	if (BlockIndex() > realBlockSize)
	{

		// 64�܂�0�Ŗ��߂�B
		for (u64 count = BlockIndex(); count < MessageBlockSize; ++count)
		{
			blockArray[count] = 0x00;
			++processByteNum;
		}

		// SHA1�ɉ��H����
		SHA1Process(&proxy);

	}

	// 56�܂�0�Ŗ��߂�B
	for (u64 count = BlockIndex(); count < realBlockSize; ++count)
	{
		blockArray[count] = 0x00;
		++processByteNum;
	}

	// ���K��8Byte�Ɋi�[����
	u64 messageLength = proxy.MessageSize * MessageLengthSize;

	// �c���8���T�C�Y�Ŗ��߂�
	u8 loopCount = 0;
	for (u64 count = realBlockSize; count < MessageBlockSize; ++count, ++loopCount)
	{
		u8 shiftBit = BitPerByte * (MessageLengthSize - loopCount - 1);
		const u8 mask = ~0;
		blockArray[count] = (messageLength >> shiftBit) & mask;
		++processByteNum;
	}

	// SHA1�ɉ��H����
	SHA1Process(&proxy);

	SHA1Result(&proxy);
}

void CSHA1::SHA1Process(FSHAProxy* _proxy)
{
	if (_proxy == nullptr) { return; }

	array<u32, TempWordNum> tempWord = {};
	auto& blockArray = _proxy->BlockArray;

	for (u8 count = 0; count < FirstWord; ++count)
	{
		auto& elem = tempWord[count];
		const u8 wordBit = WordSize * BitPerByte;
		u8 blockIndex = count * WordSize;

		for (u8 i = 0; i < WordSize; ++i)
		{
			u8 index = blockIndex + i;
			u8 shiftBit = wordBit - BitPerByte * (i + 1);
			elem |= blockArray[index] << shiftBit;
		}
	}

	for (u8 count = FirstWord; count < TempWordNum; ++count)
	{
		u32 shiftWord = tempWord[count - 3] ^ tempWord[count - 8] ^ tempWord[count - 14] ^ tempWord[count - 16];
		
		tempWord[count] = CMathUtility::CircularShift(1, shiftWord);
	}

	u8 sequence = 0;

	u32 A = _proxy->TempHash[0];
	u32 B = _proxy->TempHash[1];
	u32 C = _proxy->TempHash[2];
	u32 D = _proxy->TempHash[3];
	u32 E = _proxy->TempHash[4];

	// 0 -79 �܂ŏ璷����
	for (u8 count = 0; count < TempWordNum; ++count)
	{
		if (!m_CheckFunction[sequence](count))
		{
			if (sequence + 1 < SequenceNum)
			{
				++sequence;
			}
		}

		u32 K = m_HashConstant[sequence];
		Logic logic = m_LogicFunction[sequence];

		u32 temp = CMathUtility::CircularShift(5, A) + logic(B,C,D) + E + tempWord[count]+ K;

		E = D;
		D = C;
		C = CMathUtility::CircularShift(30, B);
		B = A;
		A = temp;
	}

	_proxy->TempHash[0] += A;
	_proxy->TempHash[1] += B;
	_proxy->TempHash[2] += C;
	_proxy->TempHash[3] += D;
	_proxy->TempHash[4] += E;
}

void CSHA1::SHA1Result(FSHAProxy* _proxy)
{
	if (!_proxy) { return; }
#if 0

	// �ꎞ�I�ɕۑ������n�b�V���l�����32bit�������o���Č���(20Byte)
	array<u8, DigestSize> digest = {};

	u8 needByte = DigestSize / SCast<u8>(_proxy->TempHash.size());
	u8 byteCount = 0;
	u8 hashIndex = 0;

	for (u8 count = 0; count < DigestSize; ++count)
	{
		u8 shiftBit = BitPerByte * (needByte - byteCount - 1);
		digest[count] = _proxy->TempHash[hashIndex] >> shiftBit;


		++byteCount;
		if (byteCount >= needByte)
		{
			byteCount = 0;
			++hashIndex;
		}

	}

	// FSHA1�\���̂ɋl�ߍ���ł���
	u8 count = 0;
	u32 size = sizeof(m_Hash.UpperByte);
	for (u8 i = 0; count < size; ++i)
	{
		u8 wordByte = BitPerByte * sizeof(m_Hash.UpperByte);
		u8 shiftBit = wordByte - BitPerByte * (i + 1);
		u64 mask = ~0;
		u64 bit = digest[count++]; // ��U64�ɕ��荞�܂Ȃ��ƁA���̂܂܃r�b�g�V�t�g�����ۂ�32�œ��ł��ɂȂ�
		m_Hash.UpperByte |= (bit << shiftBit) & mask;
	}

	size += sizeof(m_Hash.MiddleByte);
	for (u8 i = 0; count < size; ++i)
	{
		u8 wordByte = BitPerByte * sizeof(m_Hash.MiddleByte);
		u8 shiftBit = wordByte - BitPerByte * (i + 1);
		u64 mask = ~0;
		u64 bit = digest[count++];
		m_Hash.MiddleByte |= (bit << shiftBit) & mask;
	}

	size += sizeof(m_Hash.UnderByte);
	for (u8 i = count; count < size; ++i)
	{
		u8 wordByte = BitPerByte * sizeof(m_Hash.UnderByte);
		u8 shiftBit = wordByte - BitPerByte * (i + 1);
		u32 mask = ~0;
		u32 bit = digest[count++];
		m_Hash.UnderByte |= (bit << shiftBit) & mask;
	}
#endif

	auto& hash = _proxy->TempHash;

	//for (auto& elem : hash)
	//{
	//	PRINT("%x ", elem);
	//}
	//PRINT("\n");

	auto LeftShiftHash = [](u32 _hash, u8 _bit)
	{
		u64 temp = _hash;
		return temp << _bit;
	};

	m_Hash.UpperByte = LeftShiftHash(hash[0], 32);
	m_Hash.UpperByte |= hash[1];
	//PRINT("%llx ", m_Hash.UpperByte);

	m_Hash.MiddleByte = LeftShiftHash(hash[2], 32);
	m_Hash.MiddleByte |= hash[3];
	//PRINT("%llx ", m_Hash.MiddleByte);

	m_Hash.UnderByte = hash[4];
	//PRINT("%x ", m_Hash.UnderByte);
}

bool CSHA1::Check00(u32 _Num)
{
	return CMathUtility::CheckWithinRange(_Num, 0u, 20u);
}

bool CSHA1::Check20(u32 _Num)
{
	return CMathUtility::CheckWithinRange(_Num, 20u, 40u);
}

bool CSHA1::Check40(u32 _Num)
{
	return CMathUtility::CheckWithinRange(_Num, 40u, 60u);
}

bool CSHA1::Check60(u32 _Num)
{
	return CMathUtility::CheckWithinRange(_Num, 60u, 80u);
}

u32 CSHA1::Logic00(u32 _B, u32 _C, u32 _D)
{
	return (_B & _C) | ((~_B) & _D);
}

u32 CSHA1::Logic20(u32 _B, u32 _C, u32 _D)
{
	return _B ^ _C ^ _D;
}

u32 CSHA1::Logic40(u32 _B, u32 _C, u32 _D)
{
	return (_B & _C) | (_B & _D) | (_C & _D);
}

u32 CSHA1::Logic60(u32 _B, u32 _C, u32 _D)
{
	return Logic20(_B, _C, _D);
}

EDENS_NAMESPACE_END