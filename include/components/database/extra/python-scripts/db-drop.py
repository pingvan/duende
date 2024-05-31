import psycopg2

# Connect to the PostgreSQL database
connection = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende"
)

connection_test = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende_test"
)

# Create a cursor object to execute SQL queries
cursor = connection.cursor()
cursor_test = connection_test.cursor()

# Define SQL statements to create tables

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

cursor_test.execute(drop_tables)
cursor_test.execute(drop_types)

# Commit the transaction
connection.commit()
connection_test.commit()

# Close the cursor and connection
cursor.close()
connection.close()

cursor_test.close()
connection_test.close()
