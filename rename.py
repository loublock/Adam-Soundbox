import os

path = 'SD_CARD/MP3/'
count = 1

for filename in os.listdir('SD_CARD/MP3/'):
    if count > 9 and count <= 99:
        os.rename(r'SD_CARD/MP3/' + filename,r'SD_CARD/MP3/00' + str(count) + '.mp3')
    elif count > 99 and count <= 999:
        os.rename(r'SD_CARD/MP3/' + filename,r'SD_CARD/MP3/0' + str(count) + '.mp3')
    elif count > 999:
        os.rename(r'SD_CARD/MP3/' + filename,r'SD_CARD/MP3/' + str(count) + '.mp3')
    else:
        os.rename(r'SD_CARD/MP3/' + filename,r'SD_CARD/MP3/000' + str(count) + '.mp3')
    
    count += 1
