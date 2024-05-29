import psycopg2

connection = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende"
)

cursor = connection.cursor()

with open('../db-dumped-data/dumped_duende.sql', 'r') as f:
    sql_commands = f.read()
    cursor.execute(sql_commands)


connection.commit()

cursor.close()
connection.close()