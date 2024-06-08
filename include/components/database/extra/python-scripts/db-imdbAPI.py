import imdb
import pandas as pd
from datetime import datetime

ia = imdb.IMDb()

def get_movie_cover(movie_name, filmed_year):
    search_results = ia.search_movie(movie_name)
    
    for movie in search_results:
        if movie.get('year') == filmed_year:
            ia.update(movie)
            if 'cover url' in movie:
                cover_url = movie.get_fullsizeURL()
                return movie['cover url'], cover_url
            else:
                return None, None
    return None, None        

def get_actor_photo(actor_name):
    search_results = ia.search_person(actor_name)
    
    for actor in search_results:
        ia.update(actor)
        if 'headshot' in actor:
            return actor['headshot']
    return None


get_films_query = "SELECT title, release_date FROM films WHERE cover IS NULL"
insert_image_urls_query = "UPDATE films SET cover_preview_url = %s, cover_full_size_url = %s WHERE title = %s"

movie_data = pd.read_csv('../metadata/movie_data.csv')
movie_credits = pd.read_csv('../metadata/movie_credits.csv')

cnt = 0

ress = movie_credits[movie_credits['cover_preview_url'].isnull()]

for index, row in ress.iterrows():
    movie_id = row['movie_id']
    movie_row = movie_data[movie_data['id'] == movie_id]
    if not movie_row.empty:
        title = movie_row.iloc[0]['original_title']
        release_date = movie_row.iloc[0]['release_date']
        date_obj = datetime.strptime(str(release_date), '%Y-%m-%d')
        year = date_obj.year
        
        cover_url, cover_full_size_url = get_movie_cover(title, year)
        if cover_url is None or cover_full_size_url is None:
            continue
        print(cover_url, cover_full_size_url, title)
        movie_credits.loc[index, 'cover_preview_url'] = cover_url
        movie_credits.loc[index, 'cover_full_size_url'] = cover_full_size_url
        
        print('Saving')
        movie_credits.to_csv('../metadata/movie_credits.csv', index=False)
