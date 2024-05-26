from faker import Faker
import psycopg2
import random
from datetime import datetime
import hashlib

CHATS_COUNT = 50
MESSAGE_FOR_CHAT_COUNT = 100
USERS_COUNT = 100
CHATS_PER_USER = 10
VIEWS_COUNT = 100000

fake = Faker()

conn = psycopg2.connect("postgres://root:strongpassword@localhost:5432/duende")
cursor = conn.cursor()

quer_get_films_count = "SELECT COUNT(DISTINCT id) FROM films"
quer_get_users_count = "SELECT COUNT(DISTINCT id) FROM users"

quer_add_users = "INSERT INTO users (email, login) VALUES (%s, %s) RETURNING id"
quer_set_passwords = "INSERT INTO passwords (user_id, password_hash, password_salt) VALUES (%s, %s, %s)"
quer_create_chat = "INSERT INTO chats (created_at, is_group_chat, chat_name, description, users_count) VALUES (%s, %s, %s, %s, %s) RETURNING id"
quer_add_message = "INSERT INTO messages (sender_id, chat_id, sended_at, message_text_data) VALUES (%s, %s, %s, %s) RETURNING id"
quer_add_chat_membership = "INSERT INTO chat_membership (user_id, chat_id) VALUES (%s, %s)"
quer_create_form = "INSERT INTO forms (user_id) VALUES (%s) RETURNING id"
quer_add_view = "INSERT INTO views (ratio, user_id, film_id) VALUES (%s, %s, %s) RETURNING id"

user_ids = []
chat_ids = []
free_chats = []
chat_to_user = {}
chat_to_users = {}
user_chat_count = {}

cursor.execute(quer_get_films_count)
films_count = cursor.fetchone()[0]
cursor.execute(quer_get_users_count)
users_count = cursor.fetchone()[0]

for i in range(USERS_COUNT):
    cursor.execute(quer_add_users, (fake.email(), fake.name()))
    result = cursor.fetchone()
    id = 0
    if result:
        id = result[0] 
        user_ids.append(id)
    cursor.execute(quer_create_form, (id,))
    cursor.execute(quer_set_passwords, (id, hashlib.sha256("password".encode()).hexdigest(), ""))
    

for v in user_ids:
    chat_id = 0
    if not free_chats:
        cursor.execute(quer_create_chat, (fake.time(pattern='%Y-%m-%d %H:%M:%S'), False, fake.company(), fake.sentence(), 2))
        result = cursor.fetchone()
        chat_id = 0
        if result:
            chat_id = result[0]
            chat_ids.append(chat_id)
        free_chats.append(chat_id)
        cursor.execute(quer_add_chat_membership, (v, chat_id))
        chat_to_user[chat_id] = [v]
    else:
        chat_id = random.choice(free_chats)
        free_chats.remove(chat_id)
        cursor.execute(quer_add_chat_membership, (v, chat_id))
        chat_to_user[chat_id].append(v)



for chat_id in chat_ids:
    for i in range(MESSAGE_FOR_CHAT_COUNT):
        cursor.execute(quer_add_message, (random.choice(chat_to_user[chat_id]), chat_id, fake.time(pattern='%Y-%m-%d %H:%M:%S'), fake.sentence()))
        result = cursor.fetchone()  
        if result:
            id = result[0]


for i in range(VIEWS_COUNT):
    cursor.execute(quer_add_view, (random.randint(0, 10), random.choice(user_ids), random.randint(1, films_count)))

conn.commit()
conn.close()