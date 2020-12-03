USE `dbtify`;
DROP procedure IF EXISTS `get_contributors`;

DELIMITER $$
USE `dbtify`$$
CREATE PROCEDURE `get_contributors`(
	in artist_name varchar(200),
    in artist_surname varchar(200)
)
BEGIN
	declare no_more_rows boolean default false;
    declare current_other_song int;
    declare current_other_artist int;
    declare artist_song_id int;
	declare artist_id int;
    declare result varchar(400) default " ";
    
    declare artist_songs_cursor cursor for 
    select song_id from song_artist
    where song_artist.artist_id = artist_id;
    
    declare other_songs_cursor cursor for
    select song_id, song_artist.artist_id from song_artist
    where not song_artist.artist_id = artist_id; 
    
    declare continue handler for not found 
	set no_more_rows := true;
    
    select id into artist_id from artist 
    where artist.name = artist_name and
    artist.surname = artist_surname;
    
    
    open other_songs_cursor;
    
    overOthers: loop
		fetch other_songs_cursor into current_other_song, current_other_artist;
                
        if no_more_rows then
			close other_songs_cursor;
            leave overOthers;
		end if;
        
        open artist_songs_cursor;
        
        overArtist: loop
			fetch artist_songs_cursor into artist_song_id;
            
            if no_more_rows then
				set no_more_rows := false;
				close artist_songs_cursor;
				leave overArtist;
			end if;
            
            if artist_song_id = current_other_song then
				set result = concat("(", current_other_song, ",", current_other_artist, ")", "#", result);
			end if;
            
            
		end loop overArtist;
        
	end loop overOthers;
    
	select result;
END$$

DELIMITER ;

