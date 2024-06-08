import psycopg2

connection = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende"
)

cursor = connection.cursor()

clear_tables = """
TRUNCATE TABLE clients, passwords, 
tokens, forms, producers, views, films, 
producers_by_film, actors, actors_to_form, 
actors_to_film, chats, message, chat_membership, 
invites    
"""

cursor.execute(clear_tables)

connection.commit()

cursor.close()
connection.close()
