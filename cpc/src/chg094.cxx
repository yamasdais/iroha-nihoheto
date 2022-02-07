#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/rsa.h>
#include <cryptopp/default.h>
#include <cryptopp/osrng.h>

namespace fs = std::filesystem;

void encode(
    fs::path const& filePath,
    CryptoPP::BufferedTransformation const& bt
) {
    CryptoPP::FileSink file(filePath.c_str());
    bt.CopyTo(file);
    file.MessageEnd();
}

void encode_private_key(
    fs::path const& filePath,
    CryptoPP::RSA::PrivateKey const& key
) {
    CryptoPP::ByteQueue queue;
    key.DEREncodePrivateKey(queue);
    encode(filePath, queue);
}

void encode_public_key(
    fs::path const& filePath,
    CryptoPP::RSA::PublicKey const& key
) {
    CryptoPP::ByteQueue queue;
    key.DEREncodePublicKey(queue);
    encode(filePath, queue);
}

void decode(
    fs::path const& filePath,
    CryptoPP::BufferedTransformation& bt
) {
    CryptoPP::FileSource file{ filePath.c_str(), true };
    file.TransferTo(bt);
    bt.MessageEnd();
}

void decode_private_key(
    fs::path const& filePath,
    CryptoPP::RSA::PrivateKey& key
) {
    CryptoPP::ByteQueue queue;
    decode(filePath, queue);
    key.BERDecodePrivateKey(queue, false, queue.MaxRetrievable());
}

void decode_public_key(
    fs::path const& filePath,
    CryptoPP::RSA::PublicKey& key
) {
    CryptoPP::ByteQueue queue;
    decode(filePath, queue);
    key.BERDecodePublicKey(queue, false, queue.MaxRetrievable());
}

void generate_keys(
    fs::path const& privateKeyPath,
    fs::path const& publicKeyPath,
    CryptoPP::RandomNumberGenerator& rng
) {
    try {
        CryptoPP::RSA::PrivateKey rsaPrivate;
        rsaPrivate.GenerateRandomWithKeySize(rng, 3072);

        CryptoPP::RSA::PublicKey rsaPublic{ rsaPrivate };

        encode_private_key(privateKeyPath, rsaPrivate);
        encode_public_key(publicKeyPath, rsaPublic);
    } catch (CryptoPP::Exception const& ex) {
        std::cerr << ex.what() << std::endl;
    }
}

void rsa_sign_file(
    fs::path const& filePath,
    fs::path const& privateKeyPath,
    fs::path const& signaturePath,
    CryptoPP::RandomNumberGenerator& rng
) {
    CryptoPP::RSA::PrivateKey privateKey;
    decode_private_key(privateKeyPath, privateKey);
    CryptoPP::RSASSA_PKCS1v15_SHA_Signer signer(privateKey);

    CryptoPP::FileSource src{
        filePath.c_str(), true,
        new CryptoPP::SignerFilter{
            rng, signer, new CryptoPP::FileSink{
                signaturePath.c_str()
            }
        }
    };
}

bool rsa_verify_file(
    fs::path const& filePath,
    fs::path const& publicKeyPath,
    fs::path const& signaturePath
) {
    CryptoPP::RSA::PublicKey publicKey;
    decode_public_key(publicKeyPath.c_str(), publicKey);

    CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier{ publicKey };
    CryptoPP::FileSource signatureFile{ signaturePath.c_str(), true };

    if (signatureFile.MaxRetrievable() != verifier.SignatureLength())
        return false;

    CryptoPP::SecByteBlock signature{ verifier.SignatureLength() };
    signatureFile.Get(signature, signature.size());

    auto* verifierFilter = new CryptoPP::SignatureVerificationFilter{ verifier };
    verifierFilter->Put(signature, verifier.SignatureLength());

    CryptoPP::FileSource src{ filePath.c_str(), true, verifierFilter };

    return verifierFilter->GetLastResult();
}

void checkSign() {
    CryptoPP::AutoSeededRandomPool rng;
    constexpr auto privateKeyFile = "../data/rsa-private.key";
    constexpr auto publicKeyFile = "../data/rsa-public.key";
    constexpr auto targetFile = "../data/sample.txt";
    constexpr auto signFile = "../data/sample.txt.sign";

    generate_keys(privateKeyFile, publicKeyFile, rng);

    rsa_sign_file("../data/sample.txt", privateKeyFile, "../data/sample.txt.sign", rng);

    auto result = rsa_verify_file(targetFile, publicKeyFile, signFile);

    std::cout << "Verify: " << (char const*)(result ? "success" : "fail") << std::endl;
}

int main(int, char**) {
    checkSign();
    return 0;
}