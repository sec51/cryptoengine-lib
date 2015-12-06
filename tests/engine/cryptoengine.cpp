//
// Created by silenteh on 04/11/15.
//

#include <exception>

#include <gtest/gtest.h>
#include "cryptoengine_hkdf_utils.h"
#include "cryptoengine.h"
#include "cryptoengine_message.h"
#include "cryptoengine_encrypted_message.h"
#include "verification_engine.h"


namespace {

    class CryptoEngineTest : public ::testing::Test {

// The fixture for testing class Foo.
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        CryptoEngineTest() {
            // You can do set-up work for each test here.
        }

        virtual ~CryptoEngineTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Objects declared here can be used by all tests in the test case for HKDF.
    };


    TEST(CryptoEngine, SymmetricEncryptionDecryption) {

        std::string context("test-context");
        std::string clear_text("This message is just jibber jabber, can you decrypt it ?");

        // Init the crypto engine
        CryptoEngine crypto(context);

        // Encrypt the message with a randomly generated secret key
        try {
            EncryptedMessage encrypted_message = crypto.Encrypt(clear_text);
            // Make sure the encrypted text is not the clear_text
            ASSERT_NE(encrypted_message.message(), clear_text);

            // Serialize it to bytes (to simulate sending it via the network)
            std::vector<unsigned char> encrypted_message_bytes = encrypted_message.ToBytes();

            // Deserialize it back
            EncryptedMessage deserialized_encrypted_message = EncryptedMessage::FromBytes(encrypted_message_bytes);

            // Verify the encrypted_message and deserialized_encrypted_message have the same fields
            ASSERT_EQ(encrypted_message.message(),deserialized_encrypted_message.message());
            ASSERT_EQ(encrypted_message.nonce(),deserialized_encrypted_message.nonce());

            // Decrypt
            Message decrypted_message = crypto.Decrypt(encrypted_message_bytes);

            ASSERT_EQ(decrypted_message.message(), clear_text);

        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            FAIL();
        }
    }

    TEST(CryptoEngine, ASymmetricEncryptionDecryption) {

        std::string context("test-context");
        std::string context_recipient("test-context-recipient");
        std::string clear_text("This message is just jibber jabber, can you decrypt it ?");

        // Init the crypto engine
        CryptoEngine crypto(context);
        // verification engine of sender
        VerificationEngine sender_ver_engine(context);

        ASSERT_EQ(crypto.PublicKey(), sender_ver_engine.public_key());

        // Init the crypto engine of the recipient to get its public key
        CryptoEngine crypto_recipient(context_recipient);
        // verification engine of recipient
        VerificationEngine recipient_ver_engine(context_recipient);

        ASSERT_EQ(crypto_recipient.PublicKey(), recipient_ver_engine.public_key());

        // Encrypt the message with a randomly generated secret key
        try {
            EncryptedMessage encrypted_message = crypto.EncryptWithPublicKey(clear_text, recipient_ver_engine);
            // Make sure the encrypted text is not the clear_text
            ASSERT_NE(encrypted_message.message(), clear_text);

            // Serialize it to bytes (to simulate sending it via the network)
            std::vector<unsigned char> encrypted_message_bytes = encrypted_message.ToBytes();

            // Deserialize it back
            EncryptedMessage deserialized_encrypted_message = EncryptedMessage::FromBytes(encrypted_message_bytes);

            // Verify the encrypted_message and deserialized_encrypted_message have the same fields
            ASSERT_EQ(encrypted_message.message(),deserialized_encrypted_message.message());
            ASSERT_EQ(encrypted_message.nonce(),deserialized_encrypted_message.nonce());

            // Decrypt the message on the recipient side
            Message decrypted_clear_text = crypto_recipient.DecryptWithPublicKey(encrypted_message_bytes, sender_ver_engine);

            ASSERT_EQ(decrypted_clear_text.message(), clear_text);

        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            FAIL();

        }
    }

    TEST(CryptoEngine, ASymmetricEncryptionDecryptionWithSignatureVerification) {

        std::string context("test-context");
        std::string context_recipient("test-context-recipient");
        std::string clear_text("This message is just jibber jabber - can you decrypt it ?");

        // Init the crypto engine
        CryptoEngine crypto(context);
        // Verification engine sender
        VerificationEngine sender_ver_engine(context);

        ASSERT_EQ(crypto.PublicKey(), sender_ver_engine.public_key());

        // Init the crypto engine of the recipient to get its public key
        CryptoEngine crypto_recipient(context_recipient);

        // Recipient Verification engine (setup by passing directly the keys)
        VerificationEngine recipient_ver_engine(crypto_recipient.PublicKey(), crypto_recipient.SignPublicKey());

        ASSERT_EQ(crypto_recipient.PublicKey(), recipient_ver_engine.public_key());

        // Encrypt the message with a randomly generated secret key
        try {
            EncryptedMessage encrypted_message = crypto.EncryptWithPublicKeyAndSign(clear_text, recipient_ver_engine);
            // Make sure the encrypted text is not the clear_text
            ASSERT_NE(encrypted_message.message(), clear_text);

            // Serialize it to bytes (to simulate sending it via the network)
            std::vector<unsigned char> encrypted_message_bytes = encrypted_message.ToBytes();

            // Deserialize it back
            EncryptedMessage deserialized_encrypted_message = EncryptedMessage::FromBytes( encrypted_message_bytes);

            // Verify the encrypted_message and deserialized_encrypted_message have the same fields
            ASSERT_EQ(encrypted_message.message(),deserialized_encrypted_message.message());
            ASSERT_EQ(encrypted_message.nonce(),deserialized_encrypted_message.nonce());

            // Decrypt the message on the recipient side
            Message decrypted_clear_text = crypto_recipient.DecryptWithPublicKeyAndVerify(encrypted_message_bytes, sender_ver_engine);

            ASSERT_EQ(decrypted_clear_text.message(), clear_text);

        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            FAIL();
        }
    }
}