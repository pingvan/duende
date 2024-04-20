from faker import Faker
import psycopg2
import random
from datetime import datetime
import hashlib

CHATS_COUNT = 50
MESSAGE_FOR_CHAT_COUNT = 100
USERS_COUNT = 100
CHATS_PER_USER = 10

fake = Faker()

conn = psycopg2.connect("postgres://root:strongpassword@localhost:5432/duende")
cursor = conn.cursor()

quer_add_users = "INSERT INTO users (email, login) VALUES (%s, %s) RETURNING id"
quer_set_passwords = "INSERT INTO passwords (user_id, password_hash, password_salt) VALUES (%s, %s, %s)"
quer_create_chat = "INSERT INTO chats (created_at, is_group_chat, chat_name, description, users_count) VALUES (%s, %s, %s, %s, %s) RETURNING id"
quer_add_message = "INSERT INTO messages (sender_id, chat_id, sended_at, message_text_data) VALUES (%s, %s, %s, %s) RETURNING id"
quer_add_chat_membership = "INSERT INTO chat_membership (user_id, chat_id) VALUES (%s, %s)"

user_ids = []
chat_ids = []
chat_to_user = dict()

for i in range(USERS_COUNT):
    cursor.execute(quer_add_users, (fake.email(), fake.name()))
    result = cursor.fetchone()
    id = 0
    if result:
        id = result[0] 
        user_ids.append(id)
    cursor.execute(quer_set_passwords, (id, hashlib.sha256("password".encode()).hexdigest(), ""))
    

for i in range(CHATS_COUNT):
    cur_time_date = datetime.now()
    cur_time_date = cur_time_date.strftime('%Y-%m-%d %H:%M:%S')
    cursor.execute(quer_create_chat, (cur_time_date, False, fake.company(), fake.sentence(), 2))
    result = cursor.fetchone()  
    if result:
        id = result[0]
        chat_ids.append(id)

for i in range(CHATS_COUNT):
    for chat_id in chat_ids:
        chat_to_user[chat_id] = []
        for j in range(CHATS_PER_USER):
            random_user = random.choice(user_ids)
            chat_to_user[chat_id].append(random_user)
            cursor.execute(quer_add_chat_membership, (random_user, chat_id))


for chat_id in chat_ids:
    for i in range(MESSAGE_FOR_CHAT_COUNT):
        cursor.execute(quer_add_message, (random.choice(chat_to_user[chat_id]), chat_id, fake.time(pattern='%Y-%m-%d %H:%M:%S'), fake.sentence()))
        result = cursor.fetchone()  
        if result:
            id = result[0]

conn.commit()
conn.close()