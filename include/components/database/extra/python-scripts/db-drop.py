import psycopg2

connection = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende"
)

cursor = connection.cursor()

drop_tables = """
DROP TABLE IF EXISTS users, passwords, 
tokens, forms, producers, views, films, 
producers_by_film, actors, actors_to_form, 
actors_to_film, chats, messages, chat_membership, 
invites, unhandled_messages, unhandled_invites,
subscriptions, black_list, chat_events, events, chat_admins  CASCADE
"""

drop_types = """
DROP TYPE IF EXISTS genre, invite_status, event_types, event_type CASCADE
"""

cursor.execute(drop_tables)
cursor.execute(drop_types)

connection.commit()

cursor.close()
connection.close()
