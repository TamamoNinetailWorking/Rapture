
#include <string>
#include <vector>
#include <functional>

#include <eden/include/system/sha1/sha1.h>

#include <stdint.h>
/*
 * If you do not have the ISO standard stdint.h header file, then you
 * must typdef the following:
 *    name              meaning
 *  uint32_t         unsigned 32 bit integer
 *  uint8_t          unsigned 8 bit integer (i.e., unsigned char)
 *  int_least16_t    integer of >= 16 bits
 *
 */

namespace Ref
{

#ifndef _SHA_enum_
#define _SHA_enum_
	enum {
		shaSuccess = 0,
		shaNull,				/* Null pointer parameter */
		shaInputTooLong,		/* input data too long */
		shaStateError			/* called Input after Result */
	};
#endif
#define SHA1HashSize 20

	/*
	 *  This structure will hold context information for the SHA-1
	 *  hashing operation
	 */
	typedef struct SHA1Context {
		uint32_t Intermediate_Hash[SHA1HashSize / 4];	/* Message Digest	*/

		uint32_t Length_Low;			/* Message length in bits		*/
		uint32_t Length_High;			/* Message length in bits		*/

		/* Index into message block array   */
		int_least16_t Message_Block_Index;
		uint8_t Message_Block[64];		/* 512-bit message blocks		*/

		int Computed;				/* Is the digest computed?			*/
		int Corrupted;				/* Is the message digest corrupted?	*/
	} SHA1Context;

	/*
	 *  Function Prototypes
	 */
	int SHA1Reset(SHA1Context*);
	int SHA1Input(SHA1Context*, const uint8_t*, unsigned int);
	int SHA1Result(SHA1Context*, uint8_t Message_Digest[SHA1HashSize]);

	/*
 *  Define the SHA1 circular left shift macro
 */
#define SHA1CircularShift(bits,word) \
				(((word) << (bits)) | ((word) >> (32-(bits))))

 /* Local Function Prototyptes */
	void SHA1PadMessage(SHA1Context*);
	void SHA1ProcessMessageBlock(SHA1Context*);

	/*
	 *  SHA1Reset
	 *
	 *  Description:
	 *      This function will initialize the SHA1Context in preparation
	 *      for computing a new SHA1 message digest.
	 *
	 *  Parameters:
	 *      context: [in/out]
	 *          The context to reset.
	 *
	 *  Returns:
	 *      sha Error Code.
	 *
	 */
	int SHA1Reset(SHA1Context* context) {
		if (!context) {
			return shaNull;
		}

		context->Length_Low = 0;
		context->Length_High = 0;
		context->Message_Block_Index = 0;

		context->Intermediate_Hash[0] = 0x67452301;
		context->Intermediate_Hash[1] = 0xEFCDAB89;
		context->Intermediate_Hash[2] = 0x98BADCFE;
		context->Intermediate_Hash[3] = 0x10325476;
		context->Intermediate_Hash[4] = 0xC3D2E1F0;

		context->Computed = 0;
		context->Corrupted = 0;

		return shaSuccess;
	}

	/*
	 *  SHA1Result
	 *
	 *  Description:
	 *      This function will return the 160-bit message digest into the
	 *      Message_Digest array  provided by the caller.
	 *      NOTE: The first octet of hash is stored in the 0th element,
	 *            the last octet of hash in the 19th element.
	 *
	 *  Parameters:
	 *      context: [in/out]
	 *          The context to use to calculate the SHA-1 hash.
	 *      Message_Digest: [out]
	 *          Where the digest is returned.
	 *
	 *  Returns:
	 *      sha Error Code.
	 *
	 */
	int SHA1Result(SHA1Context* context, uint8_t Message_Digest[SHA1HashSize]) {
		int i;

		if (!context || !Message_Digest) {
			return shaNull;
		}

		if (context->Corrupted) {
			return context->Corrupted;
		}

		if (!context->Computed) {
			SHA1PadMessage(context);
			for (i = 0; i < 64; ++i) {
				/* message may be sensitive, clear it out */
				context->Message_Block[i] = 0;
			}
			context->Length_Low = 0;	/* and clear length */
			context->Length_High = 0;
			context->Computed = 1;
		}

		for (i = 0; i < SHA1HashSize; ++i) {
			Message_Digest[i] = context->Intermediate_Hash[i >> 2]
				>> 8 * (3 - (i & 0x03));
		}

		return shaSuccess;
	}

	/*
	 *  SHA1Input
	 *
	 *  Description:
	 *      This function accepts an array of octets as the next portion
	 *      of the message.
	 *
	 *  Parameters:
	 *      context: [in/out]
	 *          The SHA context to update
	 *      message_array: [in]
	 *          An array of characters representing the next portion of
	 *          the message.
	 *      length: [in]
	 *          The length of the message in message_array
	 *
	 *  Returns:
	 *      sha Error Code.
	 *
	 */
	int SHA1Input(SHA1Context* context, const uint8_t* message_array, unsigned length) {
		if (!length) {
			return shaSuccess;
		}

		if (!context || !message_array) {
			return shaNull;
		}

		if (context->Computed) {
			context->Corrupted = shaStateError;
			return shaStateError;
		}

		if (context->Corrupted) {
			return context->Corrupted;
		}
		while (length-- && !context->Corrupted) {
			context->Message_Block[context->Message_Block_Index++] = (*message_array & 0xFF);

			context->Length_Low += 8;
			if (context->Length_Low == 0) {
				context->Length_High++;
				if (context->Length_High == 0) {
					/* Message is too long */
					context->Corrupted = 1;
				}
			}

			if (context->Message_Block_Index == 64) {
				SHA1ProcessMessageBlock(context);
			}

			message_array++;
		}

		return shaSuccess;
	}

	/*
	 *  SHA1ProcessMessageBlock
	 *
	 *  Description:
	 *      This function will process the next 512 bits of the message
	 *      stored in the Message_Block array.
	 *
	 *  Parameters:
	 *      None.
	 *
	 *  Returns:
	 *      Nothing.
	 *
	 *  Comments:
	 *      Many of the variable names in this code, especially the
	 *      single character names, were used because those were the
	 *      names used in the publication.
	 *
	 *
	 */
	void SHA1ProcessMessageBlock(SHA1Context* context) {
		const uint32_t K[] = {			/* Constants defined in SHA-1		*/
			0x5A827999,
			0x6ED9EBA1,
			0x8F1BBCDC,
			0xCA62C1D6
		};
		int			t;					/* Loop counter						*/
		uint32_t	temp;				/* Temporary word value				*/
		uint32_t	W[80];				/* Word sequence					*/
		uint32_t	A, B, C, D, E;		/* Word buffers						*/

		/*
		 *  Initialize the first 16 words in the array W
		 */
		for (t = 0; t < 16; t++) {
			W[t] = context->Message_Block[t * 4] << 24;
			W[t] |= context->Message_Block[t * 4 + 1] << 16;
			W[t] |= context->Message_Block[t * 4 + 2] << 8;
			W[t] |= context->Message_Block[t * 4 + 3];

			PRINT("t = %d::%x ", t,W[t]);
		}

		PRINT("\n");

		for (t = 16; t < 80; t++) {
			W[t] = SHA1CircularShift(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
		}


		//for (auto& elem : W)
		//{
		//	PRINT("%x ", elem);
		//}

		//PRINT("\n");

		A = context->Intermediate_Hash[0];
		B = context->Intermediate_Hash[1];
		C = context->Intermediate_Hash[2];
		D = context->Intermediate_Hash[3];
		E = context->Intermediate_Hash[4];

		for (t = 0; t < 20; t++) {
			temp = SHA1CircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
			E = D;
			D = C;
			C = SHA1CircularShift(30, B);
			B = A;
			A = temp;
			//PRINT("Count A:%x B:%x C:%x D:%x E:%x\n", t, A, B, C, D, E);
		}

		for (t = 20; t < 40; t++) {
			temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
			E = D;
			D = C;
			C = SHA1CircularShift(30, B);
			B = A;
			A = temp;
			//PRINT("Count A:%x B:%x C:%x D:%x E:%x\n", t, A, B, C, D, E);
		}

		for (t = 40; t < 60; t++) {
			temp = SHA1CircularShift(5, A) +
				((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
			E = D;
			D = C;
			C = SHA1CircularShift(30, B);
			B = A;
			A = temp;
			//PRINT("Count A:%x B:%x C:%x D:%x E:%x\n", t, A, B, C, D, E);
		}

		for (t = 60; t < 80; t++) {
			temp = SHA1CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
			E = D;
			D = C;
			C = SHA1CircularShift(30, B);
			B = A;
			A = temp;
			//PRINT("Count A:%x B:%x C:%x D:%x E:%x\n", t, A, B, C, D, E);
		}

		context->Intermediate_Hash[0] += A;
		context->Intermediate_Hash[1] += B;
		context->Intermediate_Hash[2] += C;
		context->Intermediate_Hash[3] += D;
		context->Intermediate_Hash[4] += E;

		for (auto& elem : context->Intermediate_Hash)
		{
			PRINT("%x ,", elem);
		}
		PRINT("\n");

		context->Message_Block_Index = 0;
	}


	/*
	 *  SHA1PadMessage
	 *
	 *  Description:
	 *      According to the standard, the message must be padded to an even
	 *      512 bits.  The first padding bit must be a '1'.  The last 64
	 *      bits represent the length of the original message.  All bits in
	 *      between should be 0.  This function will pad the message
	 *      according to those rules by filling the Message_Block array
	 *      accordingly.  It will also call the ProcessMessageBlock function
	 *      provided appropriately.  When it returns, it can be assumed that
	 *      the message digest has been computed.
	 *
	 *  Parameters:
	 *      context: [in/out]
	 *          The context to pad
	 *      ProcessMessageBlock: [in]
	 *          The appropriate SHA*ProcessMessageBlock function
	 *  Returns:
	 *      Nothing.
	 *
	 */

	void SHA1PadMessage(SHA1Context* context) {
		/*
		 *  Check to see if the current message block is too small to hold
		 *  the initial padding bits and length.  If so, we will pad the
		 *  block, process it, and then continue padding into a second
		 *  block.
		 */
		if (context->Message_Block_Index > 55)
		{
			context->Message_Block[context->Message_Block_Index++] = 0x80;
			while (context->Message_Block_Index < 64)
			{
				context->Message_Block[context->Message_Block_Index++] = 0;
			}

			SHA1ProcessMessageBlock(context);

			while (context->Message_Block_Index < 56)
			{
				context->Message_Block[context->Message_Block_Index++] = 0;
			}
		}
		else
		{
			context->Message_Block[context->Message_Block_Index++] = 0x80;
			while (context->Message_Block_Index < 56)
			{
				context->Message_Block[context->Message_Block_Index++] = 0;
			}
		}

		/*
		 *  Store the message length as the last 8 octets
		 */
		context->Message_Block[56] = context->Length_High >> 24;
		context->Message_Block[57] = context->Length_High >> 16;
		context->Message_Block[58] = context->Length_High >> 8;
		context->Message_Block[59] = context->Length_High;
		context->Message_Block[60] = context->Length_Low >> 24;
		context->Message_Block[61] = context->Length_Low >> 16;
		context->Message_Block[62] = context->Length_Low >> 8;
		context->Message_Block[63] = context->Length_Low;

		SHA1ProcessMessageBlock(context);
	};

#undef SHA1CircularShift
}

using namespace std;

namespace Test
{
	// 参考
	// http://bkclass.web.fc2.com/doc_sha1.html#step4

	// 循環シフト関数
	uint32 SHA1CircularShift(uint8 _Bit, uint32 _Word)
	{
		return (_Word << _Bit) | (_Word >> (32 - _Bit));
	}

	// (B AND C) OR ((NOT B) AND D)
	uint32 SHA1_f00(uint32 _B, uint32 _C, uint32 _D)
	{
		return (_B & _C) | ((~_B) & _D);
	}

	// B XOR C XOR D
	uint32 SHA1_f20(uint32 _B, uint32 _C, uint32 _D)
	{
		return _B ^ _C ^ _D;
	}

	// (B AND C) OR (B AND D) OR (C AND D)
	uint32 SHA1_f40(uint32 _B, uint32 _C,uint32 _D)
	{
		return (_B & _C) | (_B & _D) | (_C & _D);
	}

	// B XOR C XOR D
	uint32 SHA1_f60(uint32 _B, uint32 _C, uint32 _D)
	{
		return SHA1_f20(_B, _C, _D);
	}

	typedef  function<uint32(uint32, uint32, uint32)> LogicalFunction;

	vector<LogicalFunction> logicalFunction = 
	{ 
		SHA1_f00,	/*  0～19回目の演算で使用 */
		SHA1_f20,	/* 20～39回目の演算で使用 */
		SHA1_f40,	/* 40～59回目の演算で使用 */
		SHA1_f60, 	/* 60～79回目の演算で使用 */
	};

	vector <uint32> constantWord =
	{
		0x5A827999,    /*  0～19回目の演算で使用 */
		0x6ED9EBA1,    /* 20～39回目の演算で使用 */
		0x8F1BBCDC,    /* 40～59回目の演算で使用 */
		0xCA62C1D6,	   /* 60～79回目の演算で使用 */
	};

	bool SHA1_CheckSequenceCondition(uint32 _Num,uint32 _A, uint32 _B)
	{
		return _A <= _Num && _Num < _B;
	}

	bool SHA1_Sequence00(uint32 _Num)
	{
		return SHA1_CheckSequenceCondition(_Num, 0, 20);
	}

	bool SHA1_Sequence20(uint32 _Num)
	{
		return SHA1_CheckSequenceCondition(_Num, 20, 40);
	}

	bool SHA1_Sequence40(uint32 _Num)
	{
		return SHA1_CheckSequenceCondition(_Num, 40, 60);
	}

	bool SHA1_Sequence60(uint32 _Num)
	{
		return SHA1_CheckSequenceCondition(_Num, 60, 80);
	}

	typedef function<bool(uint32)> CheckSequence;

	vector<CheckSequence> sequenceFunction=
	{
		SHA1_Sequence00,
		SHA1_Sequence20,
		SHA1_Sequence40,
		SHA1_Sequence60,

	};

	void SHA1TestMain()
	{
		// メッセージを用意する
		//const char* testMessage = "The quick brown fox jumps over the lazy dog";
		const char* testMessage = "Paste plain-text data into the below textarea or upload a file up to 10MB. Choose the mode and/or version and click the Calculate button. The resulting hashes will be shown in both hexadecimal and Base64 encoded format.If you encounter any errors with this tool please drop me a line attaching the file that causes problems.";

#if 0
		// メッセージのバイト長を切りのいい長さまで拡張する
		// >> 64の倍数になるように拡張する必要がある
		string message(testMessage);
		vector<uint8> messageVector;
		

		// まずは今の長さを計る
		uint32 messageSize = SCast<uint32>(message.size() + 1);
		PRINT("MessageSize is %d.", messageSize);

		messageVector.resize(messageSize);

		uint32 index = 0;
		for (auto& elem : message)
		{
			messageVector[++index] = elem;
			PRINT("%c", elem);
		}
		messageVector[index] = '\n';
		PRINT("\n");

		// 拡張したメッセージの最後に8Byteの整数値を追加する
		// uint32型の最大値を超えるようなことがあると正しくメッセージが
		// 変換できないわけだが、それは42億とかなので今は考えなくて良さそう

		// 64の倍数になるようにメッセージを拡張する
		// 64の倍数になる== 64で割って余りが0になる
		// 64で割った余りを64から引いた値が加算するメッセージ数
		constexpr uint32 majorRate = 64;
		constexpr uint32 messageSizeByte = 8;
		uint32 addSize = majorRate - ((messageSize + messageSizeByte) % majorRate);
		messageVector.resize(messageSize + addSize + messageSizeByte);

		for (uint32 i = 0; i < addSize; ++i)
		{
			uint32 count = messageSize + i;
			if (i == 0)
			{
				/// 元のメッセージの末尾にはまず0x80を追加する
				messageVector[count] = SCast<uint8>(0x80);
			}
			else
			{
				// それ以降は0x00を入れる
				messageVector[count] = SCast<uint8>(0x00);
			}
		}

		//messageSizeByteとして予約しておいた分を入れる
		constexpr uint32 PerByteBit = 8;
		uint64 messageBit = PerByteBit * messageSize;
		for (uint32 i = 0; i < messageSizeByte; ++i)
		{
			uint32 count = messageSize + addSize + i;

			char byte = SCast<uint8>(messageBit >> (PerByteBit * (messageSizeByte - i + 1)));
			

			messageVector[count] = byte;
		}

		for (auto& elem : messageVector)
		{
			PRINT("%d\n", elem);
		}
		PRINT("\n");
#endif

		// メッセージをまず拡張するのではなく、64Byteに区切っていくことが先
		string message(testMessage);
		//uint64 messageSize = message.size() + 1;
		uint64 messageSize = message.size();

		// メッセージを処理するための配列
		vector<char> messageArray;
		messageArray.resize(messageSize);
		uint64 index = 0;
		for (auto& elem : message)
		{
			messageArray[index++] = elem;
		}
		//messageArray[index] = '\n'; // こいつは含まないらしい


		//処理したByte数
		uint64 processByteNum = 0;

		// MessageArrayの数によって処理が変わる。
		constexpr uint8 BitPerByte = 8;
		constexpr uint8 MessageBlockSize = 64; // Byte
		constexpr uint8 MessageLengthSize = 8; // Byte

		// お尻の8Byteに格納する
		uint64 messageLength = messageSize * MessageLengthSize;

		// 64Byteに区切るための配列
		vector<uint8> blockArray;
		blockArray.resize(MessageBlockSize);

		auto BlockIndex = [&]()
		{
			return processByteNum % MessageBlockSize;
		};

		// ハッシュ加工用変数の準備
		vector<uint32> tempHash =
		{
			0x67452301,
			0xEFCDAB89,
			0x98BADCFE,
			0x10325476,
			0xC3D2E1F0,
		};

		// 加工処理
		auto SHA1Process = [&]()
		{
			constexpr uint8 WordNum = 80;
			constexpr uint8 FirstWord = 16;
			constexpr uint8 WordSize = 4;

			vector<uint32> word;
			word.resize(WordNum);

			// 0 - 15まで
			for (uint8 count = 0; count < FirstWord; ++count)
			{
				auto& elem = word[count];
				uint8 wordBit = WordSize * BitPerByte;
				uint8 blockIndex = count * WordSize;
				elem |= blockArray[blockIndex] << (wordBit - BitPerByte);
				elem |= blockArray[blockIndex + 1] << (wordBit - BitPerByte * 2);
				elem |= blockArray[blockIndex + 2] << (wordBit - BitPerByte * 3);
				elem |= blockArray[blockIndex + 3];

				PRINT("t = %d::%u ", count, elem);
			}

			PRINT("\n");

			// 16 - 79まで
			for (uint8 count = FirstWord; count < WordNum; ++count)
			{
				uint32 shiftWord = word[count - 3] ^ word[count - 8] ^ word[count - 14] ^ word[count - 16];

				word[count] = SHA1CircularShift(1, shiftWord);
			}

			//for (auto& elem : word)
			//{
			//	PRINT("%x ", elem);
			//}

			//PRINT("\n");
			
			uint8 sequence = 0;
			const uint8 MaxSequence = 4;

			uint32 a = tempHash[0];
			uint32 b = tempHash[1];
			uint32 c = tempHash[2];
			uint32 d = tempHash[3];
			uint32 e = tempHash[4];

			// 0 - 79まで冗長処理
			for (uint8 count = 0; count < WordNum; ++count)
			{
				if (!sequenceFunction[sequence](count))
				{
					if (sequence + 1 < MaxSequence)
					{
						++sequence;
					}
				}

				uint32 k = constantWord[sequence];
				LogicalFunction logic = logicalFunction[sequence];

				uint32 temp = SHA1CircularShift(5, a) + logic(b, c, d) + e + word[count] + k;

				e = d;
				d = c;
				c = SHA1CircularShift(30, b);
				b = a;
				a = temp;

				//PRINT("Count A:%x B:%x C:%x D:%x E:%x\n", count, a, b, c, d, e);
			}

			tempHash[0] += a;
			tempHash[1] += b;
			tempHash[2] += c;
			tempHash[3] += d;
			tempHash[4] += e;

			for (auto& elem : tempHash)
			{
				PRINT("%x ,", elem);
			}
			PRINT("\n");
		};

		for (auto& elem : messageArray)
		{
			uint64 count = BlockIndex();
			blockArray[count] = elem;

			PRINT("%c", blockArray[count]);

			++processByteNum;

			// 64の倍数のとき
			if ((BlockIndex()) == 0)
			{
				// SHA1に加工処理
				SHA1Process();
				PRINT("\n");
			}
		}

		// 外に出てきたときの処理数が肝心

		// まず0x80を埋めて、処理数を加算
		blockArray[BlockIndex()] = 0x80;
		++processByteNum;

		uint8 realBlockSize = MessageBlockSize - MessageLengthSize;

		// 処理した数を64で割った余りが、56より多いのとき
		if (BlockIndex() > realBlockSize)
		{

			// 64まで0で埋める。
			for (uint64 count = BlockIndex(); count < MessageBlockSize; ++count)
			{
				blockArray[count] = 0x00;
				++processByteNum;
			}

			// SHA1に加工処理
			SHA1Process();

		}

		// 56まで0で埋める。
		for (uint64 count = BlockIndex(); count < realBlockSize; ++count)
		{
			blockArray[count] = 0x00;
			++processByteNum;
		}

		// 残りの8をサイズで埋める
		uint8 loopCount = 0;
		for(uint64 count = realBlockSize ; count < MessageBlockSize; ++count,++loopCount)
		{
			uint8 shiftBit = BitPerByte * (MessageLengthSize - loopCount - 1);
			const uint8 mask = ~0;
			blockArray[count] = (messageLength >> shiftBit) & mask;
			++processByteNum;
		}

		// SHA1に加工処理
		SHA1Process();

		// 一時的に保存したハッシュ値を上位32bitずつ抜き出して結合(20Byte)
		vector<uint8> digest;
		const uint8 MessageDigestNum = 20;
		digest.resize(MessageDigestNum);

		uint8 needByte = MessageDigestNum / SCast<uint8>(tempHash.size());
		uint8 byteCount = 0;
		uint8 hashIndex = 0;

		for (uint8 count = 0; count < MessageDigestNum; ++count)
		{
			uint8 shiftBit = BitPerByte * (needByte - byteCount - 1);
			digest[count] = tempHash[hashIndex] >> shiftBit;


			++byteCount;
			if (byteCount >= needByte)
			{
				byteCount = 0;
				++hashIndex;
			}

		}


		for (auto& elem : digest)
		{
			PRINT("%02X ", elem);
		}
		PRINT("\n\n");

#if 0
		const char* resultarray[] = 
		{
			"A9 99 3E 36 47 06 81 6A BA 3E 25 71 78 50 C2 6C 9C D0 D8 9D",
			"84 98 3E 44 1C 3B D2 6E BA AE 4A A1 F9 51 29 E5 E5 46 70 F1",
			"34 AA 97 3C D4 C4 DA A4 F6 1E EB 2B DB AD 27 31 65 34 01 6F",
			"DE A3 56 A2 CD DD 90 C7 A7 EC ED C5 EB B5 63 93 4F 46 04 52"
		};

		for (auto& elem : resultarray)
		{
			PRINT("%s\n", elem);
		}
#endif
		PRINT("\n");

		{
			using namespace Ref;
			SHA1Context sha;
			SHA1Reset(&sha);

			SHA1Input(&sha, (const uint8*)testMessage, SCast<uint32>(strlen(testMessage)));
			
			uint8 Message_Digest[20];
			SHA1Result(&sha, Message_Digest);

			for (auto& elem : Message_Digest)
			{
				PRINT("%02X ", elem);
			}
			PRINT("\n\n");

		}

		{
			EDENS_NAMESPACE_USING;
			CSHA1 sha1(testMessage);
			CSHA1 test2("abc");
			if (sha1 == test2)
			{
				PRINT("Test OK;");
			}
			PRINT("\n\n");


		}
	}
};